#ifndef IDATABASESELECTQUERYJOIN_H
#define IDATABASESELECTQUERYJOIN_H

#include "idatabaseselectquery.h"

class IDatabaseSelectQueryJoin
{
public:
    virtual void singleWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equals, bool bind = true) = 0;
    virtual IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op) = 0;
};

#endif // IDATABASESELECTQUERYJOIN_H
