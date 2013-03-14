#ifndef IDATABASESELECTQUERY_H
#define IDATABASESELECTQUERY_H

#include <QString>
#include <QVariant>

class IDatabaseSelectQueryJoin;
class IDatabaseSelectQueryWhereGroup;

class IDatabaseSelectQuery
{
public:
    enum WhereOperator {
        Equal,
        NotEqual,
        LessThan,
        GreaterThan,
        LessThanOrEqual,
        GreaterThanOrEqual,
        Like,
        In,
        Is,
        NotIn
    };
    enum JoinOperator {
        And,
        Or
    };
    enum JoinType {
        Join,
        LeftJoin,
        RightJoin
    };
    enum Order {
        Ascending,
        Descending
    };

    virtual bool first() = 0;
    virtual bool next() = 0;
    virtual QVariant value(const QString &key) const = 0;

    virtual void select(const QString &column) = 0;
    virtual void select(const QStringList &columns) = 0;

    virtual void singleWhere(const QString &column, const QVariant &value, WhereOperator op = Equal, bool bind = true) = 0;
    virtual IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op) = 0;

    virtual IDatabaseSelectQueryJoin *join(const QString &table, IDatabaseSelectQuery::JoinType joinType = Join) = 0;

    virtual void groupBy(const QString &column) = 0;
    virtual void groupBy(const QStringList &columns) = 0;

    virtual void orderBy(const QString &column, IDatabaseSelectQuery::Order order = IDatabaseSelectQuery::Ascending) = 0;

    virtual void limit(int l) = 0;
    virtual void offset(int o) = 0;
};

#endif // IDATABASESELECTQUERY_H
