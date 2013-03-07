#ifndef SYNCEDDATABASEUPDATEQUERY_H
#define SYNCEDDATABASEUPDATEQUERY_H

#include "databaseupdatequery.h"
#include "isynceddatabaseupdatequery.h"

class SyncedDatabaseUpdateQuery : public DatabaseUpdateQuery, public ISyncedDatabaseUpdateQuery
{
public:
    SyncedDatabaseUpdateQuery(const QString &table, EntryType type = InsertOrUpdate, QObject *parent = 0);

    int syncWith() { return m_syncWith; }
    void setSyncWith(int clientId) { m_syncWith = clientId; }

    int groupId() { return m_groupId; }
    void setGroupId(int groupId) { m_groupId = groupId; }

    QString lastSyncId() const { return m_lastSyncId; }

protected:
    bool executeInsert();
    bool executeUpdate();
    bool executeDelete();

private:
    bool executeInsert(const QString &syncId);
    bool executeUpdate(const QString &syncId) const;
    bool executeDelete(const QString &syncId) const;

    QStringList getSyncIdsToModify() const;
    void saveSyncJournalItem(const QString &syncId, int clientRecNum = -1);

    int m_syncWith;
    int m_groupId;
    QString m_lastSyncId;
};

#endif // SYNCEDDATABASEUPDATEQUERY_H
