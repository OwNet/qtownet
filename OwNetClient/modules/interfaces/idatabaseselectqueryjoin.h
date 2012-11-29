#ifndef IDATABASESELECTQUERYJOIN_H
#define IDATABASESELECTQUERYJOIN_H

#include "idatabaseselectquery.h"

class IDatabaseSelectQueryJoin
{
public:
    virtual void where(const QString &column, const QVariant &value, IDatabaseSelectQuery::Operator op = IDatabaseSelectQuery::Equals, bool bind = true) = 0;
};

#endif // IDATABASESELECTQUERYJOIN_H
