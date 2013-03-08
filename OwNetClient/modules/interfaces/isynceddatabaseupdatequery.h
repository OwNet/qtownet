#ifndef ISYNCEDDATABASEUPDATEQUERY_H
#define ISYNCEDDATABASEUPDATEQUERY_H

#include <QString>

class ISyncedDatabaseUpdateQuery
{
public:
    virtual QString syncWith() = 0;
    virtual void setSyncWith(const QString &clientId) = 0;

    virtual int groupId() = 0;
    virtual void setGroupId(int groupId) = 0;

    virtual QString lastUid() const = 0;
};

#endif // ISYNCEDDATABASEUPDATEQUERY_H
