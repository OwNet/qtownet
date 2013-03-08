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
      m_groupId(-1)
{
}

bool SyncedDatabaseUpdateQuery::executeInsert()
{
    DatabaseSettings settings;
    int clientRecNum = settings.nextClientSyncRecordNumber();
    QString uid = QString("%1_%2")
            .arg(settings.clientId())
            .arg(clientRecNum);

    if (executeInsert(uid)) {
        saveSyncJournalItem(uid, clientRecNum);
        return true;
    }
    return false;
}

bool SyncedDatabaseUpdateQuery::executeInsert(const QString &uid)
{
    m_columns.insert("uid", uid);

    QStringList columnKeys = m_columns.keys();

    return executeQuery(QString("INSERT INTO %1 (%2) VALUES (:v_%3)")
                        .arg(table())
                        .arg(columnKeys.join(", "))
                        .arg(columnKeys.join(", :v_")));
}

bool SyncedDatabaseUpdateQuery::executeUpdate()
{
    bool success = true;
    QStringList ids = getUidsToModify();

    foreach (QString uid, ids) {
        if (!executeUpdate(uid))
            success = false;
        else
            saveSyncJournalItem(uid);
    }
    return success;
}

bool SyncedDatabaseUpdateQuery::executeUpdate(const QString &uid) const
{
    QStringList columnKeys = m_columns.keys();

    QString queryString = QString("UPDATE %1 SET").arg(table());

    for (int i = 0; i < columnKeys.count(); ++i) {
        if (i > 0)
            queryString.append(",");
        queryString.append(QString(" %1 = :v_%1").arg(columnKeys.at(i)));
    }
    queryString.append(QString(" WHERE uid = '%1'").arg(uid));

    return executeQuery(queryString);
}

bool SyncedDatabaseUpdateQuery::executeDelete()
{
    bool success = true;
    QStringList ids = getUidsToModify();

    foreach (QString uid, ids) {
        if (!executeDelete(uid))
            success = false;
        else
            saveSyncJournalItem(uid);
    }
    return success;
}

bool SyncedDatabaseUpdateQuery::executeDelete(const QString &uid) const
{
    return executeQuery(QString("DELETE FROM %1 WHERE uid = '%2'")
                        .arg(table())
                        .arg(uid));
}

void SyncedDatabaseUpdateQuery::saveSyncJournalItem(const QString &uid, int clientRecNum)
{
    m_lastUid = uid;
    if (clientRecNum < 0)
        clientRecNum = uid.split('_').last().toInt();

    DatabaseSettings settings;
    DatabaseUpdateQuery updateQuery("sync_journal");
    updateQuery.setColumnValue("client_id", settings.clientId());
    updateQuery.setColumnValue("uid", uid);
    updateQuery.setColumnValue("client_rec_num", clientRecNum);
    updateQuery.setColumnValue("date_created", timestamp());
    updateQuery.setColumnValue("table_name", table());
    updateQuery.setColumnValue("operation_type", (int)type());
    if (!m_syncWith.isEmpty())
        updateQuery.setColumnValue("sync_with", m_syncWith);
    if (m_groupId != -1)
        updateQuery.setColumnValue("group_id", m_groupId);

    updateQuery.executeQuery();

    DatabaseUpdate::setLastRecordNumber(m_groupId, clientRecNum);
}

QStringList SyncedDatabaseUpdateQuery::getUidsToModify() const
{
    if (!m_selectQuery)
        return QStringList();

    m_selectQuery->clearSelect();
    m_selectQuery->resetQuery();
    m_selectQuery->select("uid");
    QStringList ids;
    while (m_selectQuery->next())
        ids.append(m_selectQuery->value("uid").toString());

    return ids;
}
