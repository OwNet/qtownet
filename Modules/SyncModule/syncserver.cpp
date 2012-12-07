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
QVariantList SyncServer::getUpdates(const QVariantMap &clientRecordNumbers, bool syncAllGroups, int clientId)
{
    SyncLock lock;
    QObject parent;

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", &parent);
    if (!syncAllGroups) {
        IDatabaseSelectQueryWhereGroup *baseOr = query->whereGroup(IDatabaseSelectQuery::Or);
        foreach (QString groupId, clientRecordNumbers.keys())
            baseOr->where("group_id", groupId.toInt());
    }

    IDatabaseSelectQuery *journalQuery = m_proxyConnection->databaseSelect("sync_journal", &parent);
    IDatabaseSelectQueryWhereGroup *journalAnd = journalQuery->whereGroup(IDatabaseSelectQuery::And);
    IDatabaseSelectQueryWhereGroup *journalOr = NULL;
    if (query->next()) {
        journalOr = journalAnd->whereGroup(IDatabaseSelectQuery::Or);

        do {
            IDatabaseSelectQueryWhereGroup *clientGroupAnd = journalOr->whereGroup(IDatabaseSelectQuery::And);
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
    journalOr->where("sync_with", clientId);

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
 * @brief Find new items on the client to be uploaded to server
 * @param clientRecordNumbers Current sync records on the client
 * @return
 */
QVariantList SyncServer::getChangesToUpload(const QVariantList &clientRecordNumbers)
{
    if (!clientRecordNumbers.count())
        return QVariantList();
    QObject parent;

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", &parent);
    IDatabaseSelectQueryWhereGroup *whereOr = query->whereGroup(IDatabaseSelectQuery::Or);

    foreach (QVariant values, clientRecordNumbers) {
        QVariantMap valuesMap = values.toMap();
        IDatabaseSelectQueryWhereGroup *whereGroupAndClient = whereOr->whereGroup(IDatabaseSelectQuery::And);
        whereGroupAndClient->where("group_id", valuesMap.value("group_id").toInt());
        whereGroupAndClient->where("client_id", valuesMap.value("client_id").toInt());
        //whereGroupAndClient->where("last_client_rec_num", valuesMap.value("last_client_rec_num").toInt(), IDatabaseSelectQuery::LessThan);
    }

    QMap<QString, int> mappedChanges;
    while (query->next()) {
        mappedChanges.insert(QString("%1-%2")
                             .arg(query->value("group_id").toInt())
                             .arg(query->value("client_id").toInt()),
                             query->value("last_client_rec_num").toInt());
    }

    QVariantList changes;
    foreach (QVariant values, clientRecordNumbers) {
        QVariantMap valuesMap = values.toMap();
        QString key = QString("%1-%2")
                .arg(valuesMap.value("group_id").toInt())
                .arg(valuesMap.value("client_id").toInt());
        if (mappedChanges.contains(key)) {
            if (mappedChanges.value(key) < valuesMap.value("last_client_rec_num").toInt()) {
                valuesMap.insert("last_client_rec_num", mappedChanges.value(key));
                changes.append(valuesMap);
            }
        } else {
            changes.append(valuesMap);
        }
    }

    return changes;
}

/**
 * @brief Save received changes from clients
 * @param changes Journal of changes
 */
void SyncServer::saveAndApplyReceivedUpdates(const QVariantList &changes)
{
    SyncLock lock;
    QObject parent;

    // Load newest record numbers from database to currentLastRecordsMap
    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", &parent);
    QMap<QString, int> currentLastRecordsMap;
    while (query->next()) {
        currentLastRecordsMap.insert(QString("%1-%2")
                             .arg(query->value("group_id").toInt())
                             .arg(query->value("client_id").toInt()),
                             query->value("last_client_rec_num").toInt());
    }

    // Iterate through received changes
    QMap<QString, int> lastRecords;
    foreach (QVariant change, changes) {
        QVariantMap changeMap = change.toMap();
        QString key = QString("%1-%2")
                .arg(changeMap.value("group_id").toInt())
                .arg(changeMap.value("client_id").toInt());

        // Skip if there are newer items in sync journal
        if (currentLastRecordsMap.contains(key) &&
                currentLastRecordsMap.value(key) >= changeMap.value("client_rec_num").toInt())
            continue;

        bool ok = false;
        QVariant json = m_proxyConnection->fromJson(changeMap.value("content").toByteArray(), &ok);
        if (ok) {
            IDatabaseUpdate *update = m_proxyConnection->databaseUpdate(&parent);
            update->setSync(false);

            int clientRecNum = changeMap.value("client_rec_num").toInt();

            // Save to sync_journal
            IDatabaseUpdateQuery *updateQuery = update->createUpdateQuery("sync_journal", IDatabaseUpdateQuery::Insert);
            updateQuery->setColumnValue("client_id", changeMap.value("client_id").toInt());
            updateQuery->setColumnValue("client_rec_num", clientRecNum);
            updateQuery->setColumnValue("content", changeMap.value("content").toString());
            if (!changeMap.value("group_id").isNull())
                updateQuery->setColumnValue("group_id", changeMap.value("group_id").toInt());
            if (!changeMap.value("sync_with").isNull())
                updateQuery->setColumnValue("sync_with", changeMap.value("sync_with").toInt());
            updateQuery->setUpdateDates(IDatabaseUpdateQuery::DateUpdated);

            // Apply update
            update->createUpdateQuery(json.toMap());

            update->execute();
            delete update;

            if (!lastRecords.contains(key) || lastRecords.value(key) < clientRecNum)
                lastRecords.insert(key, clientRecNum);
        }
    }

    // Update client_sync_records
    foreach (QString key, lastRecords.keys()) {
        QStringList split = key.split("-");
        int groupId = split.first().toInt();
        int clientId = split.last().toInt();

        IDatabaseUpdate *update = m_proxyConnection->databaseUpdate(&parent);
        update->setSync(false);

        IDatabaseUpdateQuery *updateQuery = update->createUpdateQuery("client_sync_records", IDatabaseUpdateQuery::Detect);
        updateQuery->setColumnValue("client_id", clientId);
        updateQuery->setColumnValue("group_id", groupId);
        updateQuery->setColumnValue("last_client_rec_num", lastRecords.value(key));

        update->execute();
        delete update;
    }
}
