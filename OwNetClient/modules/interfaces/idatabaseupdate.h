#ifndef IDATABASEUPDATE_H
#define IDATABASEUPDATE_H

#include "idatabaseupdatequery.h"

class IDatabaseUpdate
{
public:
    virtual void setSync(bool sync) = 0;
    virtual IDatabaseUpdateQuery *createUpdateQuery(const QString &table, IDatabaseUpdateQuery::EntryType type = IDatabaseUpdateQuery::InsertOrUpdate) = 0;
    virtual IDatabaseUpdateQuery *createUpdateQuery(const QVariantMap &content) = 0;
    virtual int execute() = 0;
};

#endif // IDATABASEUPDATE_H
