#include "databaseselectquerywheregroup.h"

#include "databaseselectquerywhereexpression.h"

#include <QStringList>

DatabaseSelectQueryWhereGroup::DatabaseSelectQueryWhereGroup(IDatabaseSelectQuery::JoinOperator op, QObject *parent)
    : IDatabaseSelectQueryWhere(parent),
      m_operator(op)
{
}

IDatabaseSelectQueryWhereGroup *DatabaseSelectQueryWhereGroup::whereGroup(IDatabaseSelectQuery::JoinOperator op)
{
    DatabaseSelectQueryWhereGroup *group = new DatabaseSelectQueryWhereGroup(op, this);
    m_expressions.append(group);
    return group;
}

void DatabaseSelectQueryWhereGroup::where(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op, bool bind)
{
    DatabaseSelectQueryWhereExpression *expression = new DatabaseSelectQueryWhereExpression(column, value, op, bind, this);
    m_expressions.append(expression);
}

QString DatabaseSelectQueryWhereGroup::toString()
{
    QString op;
    switch (m_operator) {
    case IDatabaseSelectQuery::And:
        op = " AND ";
        break;
    case IDatabaseSelectQuery::Or:
        op = " OR ";
        break;
    }
    QStringList queryStrings;
    foreach (IDatabaseSelectQueryWhere *where, m_expressions) {
        queryStrings.append(where->toString());
    }
    return QString("(%1)").arg(queryStrings.join(op));
}

void DatabaseSelectQueryWhereGroup::bindValue(QSqlQuery *query) const
{
    foreach (IDatabaseSelectQueryWhere *where, m_expressions) {
        where->bindValue(query);
    }
}
