#include "databaseselectquerywhereexpression.h"

#include <QDateTime>
#include <QSqlQuery>

int DatabaseSelectQueryWhereExpression::m_nextId = 0;

DatabaseSelectQueryWhereExpression::DatabaseSelectQueryWhereExpression(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op, bool bind, QObject *parent) :
    IDatabaseSelectQueryWhere(parent),
    m_column(column),
    m_value(value),
    m_operator(op),
    m_bind(bind)
{
    if (m_bind)
        m_id = m_nextId++;
}

QString DatabaseSelectQueryWhereExpression::toString()
{
    if (m_bind)
        return QString("%1 %2 :%3")
                .arg(m_column)
                .arg(operatorToString())
                .arg(m_id);
    return QString("%1 %2 %3")
            .arg(m_column)
            .arg(operatorToString())
            .arg(m_value.toString());
}

void DatabaseSelectQueryWhereExpression::bindValue(QSqlQuery *query) const
{
    if (m_bind)
        query->bindValue(QString(":%1").arg(m_id), m_value);
}

QString DatabaseSelectQueryWhereExpression::operatorToString() const
{
    switch (m_operator) {
    case IDatabaseSelectQuery::Equal:
        return "=";
    case IDatabaseSelectQuery::NotEqual:
        return "<>";
    case IDatabaseSelectQuery::LessThan:
        return "<";
    case IDatabaseSelectQuery::GreaterThan:
        return ">";
    case IDatabaseSelectQuery::LessThanOrEqual:
        return "<=";
    case IDatabaseSelectQuery::GreaterThanOrEqual:
        return ">=";
    case IDatabaseSelectQuery::Like:
        return "LIKE";
    case IDatabaseSelectQuery::In:
        return "IN";
    case IDatabaseSelectQuery::Is:
        return "IS";
    }
    return "=";
}
