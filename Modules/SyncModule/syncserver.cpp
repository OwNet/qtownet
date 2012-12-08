#include "syncserver.h"

#include "iproxyconnection.h"
#include "idatabaseselectquery.h"
#include "idatabaseselectquerywheregroup.h"
#include "idatabaseupdate.h"
#include "synclock.h"

#include <QSqlQuery>
#include <QStringList>

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
QVariantList SyncServer::updates(const QVariantMap &clientRecordNumbers, bool syncAllGroups, int requestingClientId)
{
    SyncLock lock;
    QObject parent;

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", &parent);
    if (!syncAllGroups) {
        IDatabaseSelectQueryWhereGroup *baseOr = query->whereGroup(IDatabaseSelectQuery::Or);
        foreach (QString groupId, clientRecordNumbers.keys()) {
            if (groupId.isNull())
                baseOr->where("group_id", "NULL", IDatabaseSelectQuery::Is, false);
            else
                baseOr->where("group_id", groupId.toInt());
        }
    }

    IDatabaseSelectQuery *journalQuery = m_proxyConnection->databaseSelect("sync_journal", &parent);
    IDatabaseSelectQueryWhereGroup *journalAnd = journalQuery->whereGroup(IDatabaseSelectQuery::And);
    IDatabaseSelectQueryWhereGroup *journalOr = NULL;
    if (query->next()) {
        journalOr = journalAnd->whereGroup(IDatabaseSelectQuery::Or);

        do {
            IDatabaseSelectQueryWhereGroup *clientGroupAnd = journalOr->whereGroup(IDatabaseSelectQuery::And);
            if (query->value("group_id").isNull())
                clientGroupAnd->where("group_id", "NULL", IDatabaseSelectQuery::Is, false);
            else
                clientGroupAnd->where("group_id", query->value("group_id"));

            clientGroupAnd->where("client_id", query->value("client_id"));

            QString groupId = query->value("group_id").toString();
            QString clientId = query->value("client_id").toString();

            if (clientRecordNumbers.contains(groupId)) {
                QVariantMap groupClients = clientRecordNumbers.value(groupId).toMap();
                if (groupClients.contains(clientId) &&
                        query->value("last_client_rec_num").toInt() > groupClients.value(clientId).toInt()) {
                    clientGroupAnd->where("client_rec_num", groupClients.value(clientId).toInt(), IDatabaseSelectQuery::GreaterThan);
                }
            }
        } while (query->next());
    }
    journalOr = journalAnd->whereGroup(IDatabaseSelectQuery::Or);
    journalOr->where("sync_with", "NULL", IDatabaseSelectQuery::Is, false);
    journalOr->where("sync_with", requestingClientId);

    journalAnd->where("client_id", requestingClientId, IDatabaseSelectQuery::NotEqual);

    QVariantList updates;
    while (journalQuery->next()) {
        QVariantMap update;
        update.insert("client_id", journalQuery->value("client_id"));
        update.insert("client_rec_num", journalQuery->value("client_rec_num"));
        update.insert("content", journalQuery->value("content"));
        update.insert("group_id", journalQuery->value("group_id"));
        update.insert("sync_with", journalQuery->value("sync_with"));
        update.insert("date_created", journalQuery->value("date_created"));
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

        bool ok = false;
        QVariant json = m_proxyConnection->fromJson(changeMap.value("content").toByteArray(), &ok);
        if (ok) {
            IDatabaseUpdate *update = m_proxyConnection->databaseUpdate(&parent);
            update->setSync(false);

            int clientRecNum = changeMap.value("client_rec_num").toInt();

            /// Save to sync_journal
            IDatabaseUpdateQuery *updateQuery = update->createUpdateQuery("sync_journal", IDatabaseUpdateQuery::Insert);

            if (!changeMap.value("client_id").isNull())
                updateQuery->setColumnValue("client_id", changeMap.value("client_id").toInt());

            updateQuery->setColumnValue("client_rec_num", clientRecNum);
            updateQuery->setColumnValue("content", changeMap.value("content").toString());

            if (!changeMap.value("group_id").isNull())
                updateQuery->setColumnValue("group_id", changeMap.value("group_id").toInt());

            if (!changeMap.value("sync_with").isNull())
                updateQuery->setColumnValue("sync_with", changeMap.value("sync_with").toInt());

            updateQuery->setUpdateDates(IDatabaseUpdateQuery::DateCreated);

            /// Apply update
            QVariantList updateQueries = json.toList();
            foreach (QVariant updateQuery, updateQueries) {
                update->createUpdateQuery(updateQuery.toMap());
            }

            update->execute();

            if (!lastRecords.contains(key) || lastRecords.value(key) < clientRecNum)
                lastRecords.insert(key, clientRecNum);
        }
    }

    /// Update client_sync_records
    foreach (QString key, lastRecords.keys()) {
        QStringList split = key.split("-");
        QString groupId = split.first();
        QString clientId = split.last();

        QString where;
        if (groupId.isEmpty())
            where += "group_id IS NULL";
        else
            QString("group_id = %1").arg(groupId.toInt());
        where += " AND ";
        if (clientId.isEmpty())
            where += "client_id IS NULL";
        else
            where += QString("client_id = %1").arg(clientId.toInt());

        QSqlQuery query;
        query.prepare(QString("SELECT * FROM client_sync_records WHERE %1").arg(where));

        if (query.exec() && query.first()) {
            QSqlQuery update;
            update.prepare(QString("UPDATE client_sync_records SET last_client_rec_num = %1 WHERE %2")
                           .arg(lastRecords.value(key))
                           .arg(where));
            update.exec();
        } else {
            QStringList columns;
            QStringList values;
            if (!groupId.isEmpty()) {
                columns.append("group_id");
                values.append(QString::number(groupId.toInt()));
            } if (!clientId.isEmpty()) {
                columns.append("client_id");
                values.append(QString::number(clientId.toInt()));
            }
            columns.append("last_client_rec_num");
            values.append(QString::number(lastRecords.value(key)));

            QSqlQuery insert;
            insert.prepare(QString("INSERT INTO client_sync_records (%1) VALUES (%2)")
                    .arg(columns.join(", "))
                    .arg(values.join(", ")));
            insert.exec();
        }
    }
}
