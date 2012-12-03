#ifndef DATABASESELECTQUERYWHEREGROUP_H
#define DATABASESELECTQUERYWHEREGROUP_H

#include "idatabaseselectquerywheregroup.h"
#include "idatabaseselectquerywhere.h"

class DatabaseSelectQueryWhereGroup : public IDatabaseSelectQueryWhereGroup, public IDatabaseSelectQueryWhere
{
public:
    DatabaseSelectQueryWhereGroup(IDatabaseSelectQuery::JoinOperator op, QObject *parent = 0);

    IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op);
    void where(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equal, bool bind = true);

    QString toString();
    void bindValue(QSqlQuery *query) const;

private:
    IDatabaseSelectQuery::JoinOperator m_operator;
    QList<IDatabaseSelectQueryWhere *> m_expressions;
};

#endif // DATABASESELECTQUERYWHEREGROUP_H
