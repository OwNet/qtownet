#ifndef IDATABASESELECTQUERYWHEREGROUP_H
#define IDATABASESELECTQUERYWHEREGROUP_H

#include "idatabaseselectquery.h"

class IDatabaseSelectQueryWhereGroup
{
public:
    virtual IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op) = 0;
    virtual void where(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equal, bool bind = true) = 0;
};

#endif // IDATABASESELECTQUERYWHEREGROUP_H
