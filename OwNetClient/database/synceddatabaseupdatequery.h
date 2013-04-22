#ifndef SYNCEDDATABASEUPDATEQUERY_H
#define SYNCEDDATABASEUPDATEQUERY_H

#include "databaseupdatequery.h"
#include "isynceddatabaseupdatequery.h"

class SyncedDatabaseUpdateQuery : public DatabaseUpdateQuery, public ISyncedDatabaseUpdateQuery
{
public:
    SyncedDatabaseUpdateQuery(const QString &table, EntryType type = InsertOrUpdate, QObject *parent = 0);

    QString syncWith() { return m_syncWith; }
    void setSyncWith(const QString &clientId) { m_syncWith = clientId; }

    int groupId() { return m_groupId; }
    void setGroupId(int groupId) { m_groupId = groupId; }

    QString lastUid() const { return m_lastUid; }

    ISyncedDatabaseUpdateQuery *syncedQuery() { return this; }

protected:
    bool executeInsert();
    bool executeUpdate();
    bool executeDelete();

private:
    bool executeInsert(const QString &uid);
    bool executeUpdate(const QString &uid) const;
    bool executeDelete(const QString &uid) const;

    QStringList getUidsToModify() const;
    void saveSyncJournalItem(const QString &uid, int clientRecNum);

    QString m_syncWith;
    int m_groupId;
    QString m_lastUid;
};

#endif // SYNCEDDATABASEUPDATEQUERY_H
