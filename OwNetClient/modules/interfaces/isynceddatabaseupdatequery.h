#ifndef ISYNCEDDATABASEUPDATEQUERY_H
#define ISYNCEDDATABASEUPDATEQUERY_H

#include <QString>

class ISyncedDatabaseUpdateQuery
{
public:
    virtual int syncWith() = 0;
    virtual void setSyncWith(int clientId) = 0;

    virtual int groupId() = 0;
    virtual void setGroupId(int groupId) = 0;

    virtual QString lastSyncId() const = 0;
};

#endif // ISYNCEDDATABASEUPDATEQUERY_H
