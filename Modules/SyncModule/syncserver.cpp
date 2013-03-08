#include "syncserver.h"

#include "iproxyconnection.h"
#include "idatabaseselectquery.h"
#include "idatabaseselectquerywheregroup.h"
#include "idatabaseupdatequery.h"
#include "synclock.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QStringList>
#include <QDebug>

SyncServer::SyncServer(IProxyConnection *proxyConnection, QObject *parent)
    : QObject(parent),
      m_proxyConnection(proxyConnection)
{
}

/**
 * @brief Return updates not present on the client
 * @param clientRecordNumbers Current state of records on the client
 * @param syncAllGroups Option to sync all groups or just the ones in clientRecordNumbers
 * @param clientId ID of the client
 * @return Updates to be applied on the client
 */
QVariantList SyncServer::updates(const QVariantMap &clientRecordNumbers, bool syncAllGroups, const QString &requestingClientId)
{
    SyncLock lock;
    QObject parent;

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", &parent);
    if (!syncAllGroups) {
        IDatabaseSelectQueryWhereGroup *baseOr = query->whereGroup(IDatabaseSelectQuery::Or);

        if (clientRecordNumbers.keys().count() > 0) {
            foreach (QString groupId, clientRecordNumbers.keys()) {
                if (groupId.isEmpty())
                    baseOr->where("group_id", "NULL", IDatabaseSelectQuery::Is, false);
                else
                    baseOr->where("group_id", groupId.toInt());
            }
        } else {
            baseOr->where("group_id", "NULL", IDatabaseSelectQuery::Is, false);
        }
    }
    query->orderBy("group_id");

    bool foundUpdates = false;
    IDatabaseSelectQuery *journalQuery = m_proxyConnection->databaseSelect("sync_journal", &parent);
    IDatabaseSelectQueryWhereGroup *journalAnd = journalQuery->whereGroup(IDatabaseSelectQuery::And);
    IDatabaseSelectQueryWhereGroup *journalOr = NULL;
    if (query->next()) {
        journalOr = journalAnd->whereGroup(IDatabaseSelectQuery::Or);
        QString lastGroupId;
        IDatabaseSelectQueryWhereGroup *groupAnd = NULL;

        do {
            QString groupId = query->value("group_id").toString();
            QString clientId = query->value("client_id").toString();
            int last_client_rec_num = -1;

            if (clientRecordNumbers.contains(groupId)) {
                QVariantMap groupClients = clientRecordNumbers.value(groupId).toMap();
                if (groupClients.contains(clientId)) {
                    last_client_rec_num = groupClients.value(clientId).toInt();
                }
            }

            if (!groupAnd || lastGroupId != groupId) {
                groupAnd = journalOr->whereGroup(IDatabaseSelectQuery::And);

                if (query->value("group_id").isNull())
                    groupAnd->where("group_id", "NULL", IDatabaseSelectQuery::Is, false);
                else
                    groupAnd->where("group_id", query->value("group_id"));

                lastGroupId = groupId;
            }

            if (last_client_rec_num > -1) {
                IDatabaseSelectQueryWhereGroup *clientOr = groupAnd->whereGroup(IDatabaseSelectQuery::Or);

                clientOr->where("client_id", query->value("client_id"), IDatabaseSelectQuery::NotEqual);
                clientOr->where("client_rec_num", last_client_rec_num, IDatabaseSelectQuery::GreaterThan);
            }
            foundUpdates = true;
        } while (query->next());
    }

    QVariantList updates;
    if (!foundUpdates)
        return updates;

    journalOr = journalAnd->whereGroup(IDatabaseSelectQuery::Or);
    journalOr->where("sync_with", "NULL", IDatabaseSelectQuery::Is, false);
    journalOr->where("sync_with", requestingClientId);

    if (!requestingClientId.isEmpty())
        journalAnd->where("client_id", requestingClientId, IDatabaseSelectQuery::NotEqual);

    while (journalQuery->next()) {
        QVariantMap update;
        update.insert("client_id", journalQuery->value("client_id"));
        update.insert("client_rec_num", journalQuery->value("client_rec_num"));
        update.insert("table_name", journalQuery->value("table_name"));
        update.insert("uid", journalQuery->value("uid"));
        update.insert("operation_type", journalQuery->value("operation_type"));
        update.insert("group_id", journalQuery->value("group_id"));
        update.insert("sync_with", journalQuery->value("sync_with"));
        update.insert("date_created", journalQuery->value("date_created"));

        if ((IDatabaseUpdateQuery::EntryType)journalQuery->value("operation_type").toInt() == IDatabaseUpdateQuery::InsertOrUpdate) {
            QVariantMap columns;

            QSqlQuery recordQuery;
            recordQuery.prepare(QString("SELECT * FROM %1 WHERE uid = :uid")
                          .arg(journalQuery->value("table_name").toString()));
            recordQuery.bindValue(":uid", journalQuery->value("uid").toString());
            if (recordQuery.exec() && recordQuery.next()) {
                QSqlRecord record = recordQuery.record();
                for (int i = 0; i < record.count(); ++i) {
                    QSqlField field = record.field(i);
                    if (!field.name().startsWith('_'))
                        columns.insert(field.name(), recordQuery.value(i));
                }
            } else {
                continue;
            }

            update.insert("columns", columns);
        }
        updates.append(update);
    }

    return updates;
}

/**
 * @brief Get current state of applied sync updates
 * @return Current state of applied sync updates
 */
QVariantMap SyncServer::clientRecordNumbers()
{
    QObject parent;
    QVariantMap updates;
    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", &parent);

    while (query->next()) {
        QVariantMap groupClients;
        QString groupId = query->value("group_id").toString();

        if (updates.contains(groupId))
            groupClients = updates.value(groupId).toMap();

        groupClients.insert(query->value("client_id").toString(), query->value("last_client_rec_num").toInt());
        updates.insert(groupId, groupClients);
    }
    return updates;
}

/**
 * @brief Save received changes from clients
 * @param changes Journal of changes
 */
void SyncServer::saveAndApplyUpdates(const QVariantList &changes)
{
    if (!changes.count())
        return;

    SyncLock lock;
    QObject parent;

    /// Load newest record numbers from database to currentLastRecordsMap
    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", &parent);
    QMap<QString, int> currentLastRecordsMap;
    while (query->next()) {
        currentLastRecordsMap.insert(QString("%1-%2")
                             .arg(query->value("group_id").toString())
                             .arg(query->value("client_id").toString()),
                             query->value("last_client_rec_num").toInt());
    }

    /// Iterate through received changes
    QMap<QString, int> lastRecords;
    foreach (QVariant change, changes) {
        QVariantMap changeMap = change.toMap();
        QString key = QString("%1-%2")
                .arg(changeMap.value("group_id").toString())
                .arg(changeMap.value("client_id").toString());

        /// Skip if there are newer items in sync journal
        if (currentLastRecordsMap.contains(key) &&
                currentLastRecordsMap.value(key) >= changeMap.value("client_rec_num").toInt())
            continue;

        int clientRecNum = changeMap.value("client_rec_num").toInt();

        /// Save to sync_journal
        IDatabaseUpdateQuery *updateJournalQuery = m_proxyConnection->databaseUpdateQuery("sync_journal", &parent, false);

        updateJournalQuery->setColumnValue("client_id", changeMap.value("client_id"));
        updateJournalQuery->setColumnValue("client_rec_num", clientRecNum);
        updateJournalQuery->setColumnValue("table_name", changeMap.value("table_name").toString());
        updateJournalQuery->setColumnValue("uid", changeMap.value("uid").toString());
        updateJournalQuery->setColumnValue("operation_type", changeMap.value("operation_type").toInt());

        if (!changeMap.value("group_id").isNull())
            updateJournalQuery->setColumnValue("group_id", changeMap.value("group_id").toInt());

        if (!changeMap.value("sync_with").isNull())
            updateJournalQuery->setColumnValue("sync_with", changeMap.value("sync_with").toInt());

        updateJournalQuery->setUpdateDates(IDatabaseUpdateQuery::DateCreated);
        bool ok = updateJournalQuery->executeQuery();

        /// Apply update
        IDatabaseUpdateQuery *executeUpdateQuery = m_proxyConnection->databaseUpdateQuery(changeMap.value("table_name").toString(), &parent, false);
        executeUpdateQuery->setType((IDatabaseUpdateQuery::EntryType)changeMap.value("operation_type").toInt());
        if (changeMap.contains("columns")) {
            QVariantMap columns = changeMap.value("columns").toMap();
            foreach (QString columnName, columns.keys()) {
                executeUpdateQuery->setColumnValue(columnName, columns.value(columnName));
            }
        }
        executeUpdateQuery->singleWhere("uid", changeMap.value("uid").toString());
        ok = executeUpdateQuery->executeQuery();

        if (!lastRecords.contains(key) || lastRecords.value(key) < clientRecNum)
            lastRecords.insert(key, clientRecNum);
    }

    /// Update client_sync_records
    foreach (QString key, lastRecords.keys()) {
        QStringList split = key.split("-");
        QString groupId = split.first();
        QString clientId = split.last();

        IDatabaseUpdateQuery *updateQuery = m_proxyConnection->databaseUpdateQuery("client_sync_records", &parent, false);
        IDatabaseSelectQueryWhereGroup *where = updateQuery->whereGroup(IDatabaseSelectQuery::And);

        if (groupId.isEmpty())
            where->where("group_id", "NULL", IDatabaseSelectQuery::Is, false);
        else
            where->where("group_id", groupId.toInt());

        where->where("client_id", clientId);

        updateQuery->setColumnValue("last_client_rec_num", lastRecords.value(key));
        updateQuery->setColumnValue("client_id", clientId);

        if (!groupId.isEmpty())
            updateQuery->setColumnValue("group_id", groupId.toInt());

        updateQuery->executeQuery();
    }
}
