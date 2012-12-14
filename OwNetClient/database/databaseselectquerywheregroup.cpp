#include "databaseselectquerywheregroup.h"

#include "databaseselectquerywhereexpression.h"

#include <QStringList>

DatabaseSelectQueryWhereGroup::DatabaseSelectQueryWhereGroup(IDatabaseSelectQuery::JoinOperator op, QObject *parent)
    : IDatabaseSelectQueryWhere(parent),
      m_operator(op)
{
}

/**
 * @brief Create a nested group of where clauses with custom operation.
 * @param op
 * @return
 */
IDatabaseSelectQueryWhereGroup *DatabaseSelectQueryWhereGroup::whereGroup(IDatabaseSelectQuery::JoinOperator op)
{
    DatabaseSelectQueryWhereGroup *group = new DatabaseSelectQueryWhereGroup(op, this);
    m_expressions.append(group);
    return group;
}

/**
 * @brief Create a single where expression
 * @param column Name of the attribute
 * @param value Value of the attribute
 * @param op Type of operation
 * @param bind True if the value should be escaped
 */
void DatabaseSelectQueryWhereGroup::where(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op, bool bind)
{
    DatabaseSelectQueryWhereExpression *expression = new DatabaseSelectQueryWhereExpression(column, value, op, bind, this);
    m_expressions.append(expression);
}

/**
 * @brief Converts the expression to string as it would appear in SQL query.
 * @return String representation of the where expression.
 */
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

/**
 * @brief Bind values of the attribute in where clauses of the SQL query.
 * @param query SQL query object
 */
void DatabaseSelectQueryWhereGroup::bindValue(QSqlQuery *query) const
{
    foreach (IDatabaseSelectQueryWhere *where, m_expressions) {
        where->bindValue(query);
    }
}
