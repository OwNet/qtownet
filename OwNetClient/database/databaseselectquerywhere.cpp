#include "databaseselectquerywhere.h"

#include <QDateTime>
#include <QSqlQuery>

int DatabaseSelectQueryWhere::m_nextId = 0;

DatabaseSelectQueryWhere::DatabaseSelectQueryWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::Operator op, bool bind, QObject *parent) :
    QObject(parent),
    m_column(column),
    m_value(value),
    m_operator(op),
    m_bind(bind)
{
    if (m_bind)
        m_id = m_nextId++;
}

QString DatabaseSelectQueryWhere::toString() const
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

void DatabaseSelectQueryWhere::bindValue(QSqlQuery *query) const
{
    if (m_bind)
        query->bindValue(QString(":%1").arg(m_id), m_value);
}

QString DatabaseSelectQueryWhere::operatorToString() const
{
    switch (m_operator) {
    case IDatabaseSelectQuery::Equals:
        return "=";
    case IDatabaseSelectQuery::NotEquals:
        return "<>";
    case IDatabaseSelectQuery::LessThan:
        return "<";
    case IDatabaseSelectQuery::GreaterThan:
        return ">";
    case IDatabaseSelectQuery::Like:
        return "LIKE";
    case IDatabaseSelectQuery::In:
        return "IN";
    }
    return "=";
}
