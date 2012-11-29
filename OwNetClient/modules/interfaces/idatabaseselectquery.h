#ifndef IDATABASESELECTQUERY_H
#define IDATABASESELECTQUERY_H

#include <QString>
#include <QVariant>

class IDatabaseSelectQueryJoin;

class IDatabaseSelectQuery
{
public:
    enum Operator {
        Equals,
        NotEquals,
        LessThan,
        GreaterThan,
        Like,
        In
    };
    enum JoinType {
        Join,
        LeftJoin,
        RightJoin
    };

    virtual bool first() = 0;
    virtual bool next() = 0;
    virtual QVariant value(const QString &key) const = 0;

    virtual void select(const QString &column) = 0;
    virtual void select(const QStringList &columns) = 0;

    virtual void where(const QString &column, const QVariant &value, Operator op = Equals, bool bind = true) = 0;

    virtual IDatabaseSelectQueryJoin *join(const QString &table, IDatabaseSelectQuery::JoinType joinType = Join) = 0;

    virtual void groupBy(const QString &column) = 0;
    virtual void groupBy(const QStringList &columns) = 0;

    virtual void orderBy(const QString &column) = 0;
    virtual void orderBy(const QStringList &columns) = 0;

    virtual void limit(int l) = 0;
    virtual void offset(int o) = 0;
};

#endif // IDATABASESELECTQUERY_H
