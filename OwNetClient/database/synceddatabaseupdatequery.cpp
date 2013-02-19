#include "synceddatabaseupdatequery.h"

#include "databasesettings.h"
#include "databaseupdate.h"
#include "databaseselectquery.h"
#include "messagehelper.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QStringList>

SyncedDatabaseUpdateQuery::SyncedDatabaseUpdateQuery(const QString &table, IDatabaseUpdateQuery::EntryType type, QObject *parent)
    : DatabaseUpdateQuery(table, type, parent),
      m_syncWith(-1),
      m_groupId(-1)
{
}

bool SyncedDatabaseUpdateQuery::executeInsert()
{
    DatabaseSettings settings;
    int clientRecNum = settings.nextClientSyncRecordNumber();
    QString syncId = QString("%1/%2")
            .arg(settings.clientId())
            .arg(clientRecNum);

    if (executeInsert(syncId)) {
        saveSyncJournalItem(syncId, clientRecNum);
        return true;
    }
    return false;
}

bool SyncedDatabaseUpdateQuery::executeInsert(const QString &syncId)
{
    m_columns.insert("sync_id", syncId);

    QStringList columnKeys = m_columns.keys();

    return executeQuery(QString("INSERT INTO %1 (%2) VALUES (:v_%3)")
                        .arg(table())
                        .arg(columnKeys.join(", "))
                        .arg(columnKeys.join(", :v_")));
}

bool SyncedDatabaseUpdateQuery::executeUpdate()
{
    bool success = true;
    QStringList ids = getSyncIdsToModify();

    foreach (QString syncId, ids) {
        if (!executeUpdate(syncId))
            success = false;
        else
            saveSyncJournalItem(syncId);
    }
    return success;
}

bool SyncedDatabaseUpdateQuery::executeUpdate(const QString &syncId) const
{
    QStringList columnKeys = m_columns.keys();

    QString queryString = QString("UPDATE %1 SET").arg(table());

    for (int i = 0; i < columnKeys.count(); ++i) {
        if (i > 0)
            queryString.append(",");
        queryString.append(QString(" %1 = :v_%1").arg(columnKeys.at(i)));
    }
    queryString.append(QString(" WHERE sync_id = '%1'").arg(syncId));

    return executeQuery(queryString);
}

bool SyncedDatabaseUpdateQuery::executeDelete()
{
    bool success = true;
    QStringList ids = getSyncIdsToModify();

    foreach (QString syncId, ids) {
        if (!executeDelete(syncId))
            success = false;
        else
            saveSyncJournalItem(syncId);
    }
    return success;
}

bool SyncedDatabaseUpdateQuery::executeDelete(const QString &syncId) const
{
    return executeQuery(QString("DELETE FROM %1 WHERE sync_id = '%2'")
                        .arg(table())
                        .arg(syncId));
}

void SyncedDatabaseUpdateQuery::saveSyncJournalItem(const QString &syncId, int clientRecNum) const
{
    DatabaseSettings settings;
    QStringList columns;
    columns << "client_id"
            << "client_rec_num"
            << "date_created"
            << "operation_type"
            << "sync_id"
            << "table_name";
    if (m_syncWith != -1)
        columns.append("sync_with");
    if (m_groupId != -1)
        columns.append("group_id");

    QSqlQuery query;
    query.prepare(QString("INSERT INTO sync_journal (%1) VALUES (:%2)")
                  .arg(columns.join(", "))
                  .arg(columns.join(", :")));

    query.bindValue(":client_id", settings.clientId());
    query.bindValue(":sync_id", syncId);
    query.bindValue(":client_rec_num", clientRecNum);
    query.bindValue(":date_created", timestamp());
    query.bindValue(":table_name", table());
    query.bindValue(":operation_type", (int)type());
    if (m_syncWith != -1)
        query.bindValue(":sync_with", m_syncWith);
    if (m_groupId != -1)
        query.bindValue(":group_id", m_groupId);

    if (!query.exec()) {
        MessageHelper::debug(query.lastQuery());
        MessageHelper::debug(query.lastError().text());
    }

    if (clientRecNum == -1)
        clientRecNum = settings.nextClientSyncRecordNumber();
    DatabaseUpdate::setLastRecordNumber(m_groupId, clientRecNum);
}

QStringList SyncedDatabaseUpdateQuery::getSyncIdsToModify() const
{
    if (!m_selectQuery)
        return QStringList();

    m_selectQuery->clearSelect();
    m_selectQuery->resetQuery();
    m_selectQuery->select("sync_id");
    QStringList ids;
    while (m_selectQuery->next())
        ids.append(m_selectQuery->value("sync_id").toString());

    return ids;
}
