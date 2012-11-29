#include "syncserver.h"

#include "iproxyconnection.h"
#include "idatabaseselectquery.h"
#include "idatabaseselectquerywheregroup.h"
#include "idatabaseupdate.h"

#include <QSqlQuery>

SyncServer *SyncServer::m_instance = 0;

SyncServer::SyncServer()
    : m_proxyConnection(NULL)
{
}

QVariantList SyncServer::getUpdates(const QVariantMap &clientRecordNumbers, bool syncAllGroups, int clientId)
{
    QMutexLocker locker(&m_syncLock);

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", this);
    if (!syncAllGroups) {
        IDatabaseSelectQueryWhereGroup *baseOr = query->whereGroup(IDatabaseSelectQuery::Or);
        foreach (QString groupId, clientRecordNumbers.keys())
            baseOr->where("group_id", groupId.toInt());
    }

    IDatabaseSelectQuery *journalQuery = m_proxyConnection->databaseSelect("sync_journal", this);
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

QVariantList SyncServer::getChangesToUpload(const QVariantList &clientRecordNumbers)
{
    if (!clientRecordNumbers.count())
        return QVariantList();

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", this);
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

void SyncServer::uploadChanges(const QVariantList &changes)
{
    QMutexLocker locker(&m_syncLock);

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", this);

    QMap<QString, int> currentLastRecordsMap;
    while (query->next()) {
        currentLastRecordsMap.insert(QString("%1-%2")
                             .arg(query->value("group_id").toInt())
                             .arg(query->value("client_id").toInt()),
                             query->value("last_client_rec_num").toInt());
    }

    foreach (QVariant change, changes) {
        QVariantMap changeMap = change.toMap();
        QString key = QString("%1-%2")
                .arg(changeMap.value("group_id").toInt())
                .arg(changeMap.value("client_id").toInt());

        if (currentLastRecordsMap.contains(key) &&
                currentLastRecordsMap.value(key) >= changeMap.value("client_rec_num").toInt())
            continue;

        IDatabaseUpdate *update = m_proxyConnection->databaseUpdate(this);
        update->setSync(false);

        IDatabaseUpdateQuery *updateQuery = update->createUpdateQuery("sync_journal", IDatabaseUpdateQuery::Insert);
        updateQuery->setColumnValue("client_id", changeMap.value("client_id").toInt());
        updateQuery->setColumnValue("client_rec_num", changeMap.value("client_rec_num").toInt());
        updateQuery->setColumnValue("content", changeMap.value("content").toString());
        if (!changeMap.value("group_id").isNull())
            updateQuery->setColumnValue("group_id", changeMap.value("group_id").toInt());
        if (!changeMap.value("sync_with").isNull())
            updateQuery->setColumnValue("sync_with", changeMap.value("sync_with").toInt());
        updateQuery->setUpdateDates(IDatabaseUpdateQuery::DateUpdated);

        updateQuery = update->createUpdateQuery(m_proxyConnection->fromJson(changeMap.value("content").toByteArray()).toMap());
    }
}
