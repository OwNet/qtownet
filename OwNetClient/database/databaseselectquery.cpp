#include "databaseselectquery.h"

#include "databaseselectqueryjoin.h"
#include "databaseselectquerywhere.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

DatabaseSelectQuery::DatabaseSelectQuery(const QString &table, QObject *parent) :
    QObject(parent),
    m_table(table),
    m_limit(-1),
    m_offset(-1),
    m_executed(false)
{
    m_query = new QSqlQuery;
}

DatabaseSelectQuery::~DatabaseSelectQuery()
{
    delete m_query;
}

bool DatabaseSelectQuery::execute()
{
    m_executed = true;

    QString queryString = QString("SELECT %1 FROM %2")
            .arg(m_selectColumns.count() ? m_selectColumns.join(", ") : "*")
            .arg(m_table);

    foreach (DatabaseSelectQueryJoin *join, m_joins) {
        queryString.append(" " + join->toString());
    }

    if (m_wheres.count()) {
        queryString.append(" WHERE");
        foreach (DatabaseSelectQueryWhere *where, m_wheres) {
            queryString.append(" " + where->toString());
        }
    }

    if (m_groupByColumns.count()) {
        queryString.append(" GROUP BY ");
        queryString.append(m_groupByColumns.join(", "));
    }

    if (m_orderByColumns.count()) {
        queryString.append(" ORDER BY ");
        queryString.append(m_orderByColumns.join(", "));
    }

    if (m_limit >= 0)
        queryString.append(QString(" LIMIT %1").arg(m_limit));

    if (m_offset >= 0)
        queryString.append(QString(" OFFSET %1").arg(m_offset));

    m_query->prepare(queryString);

    foreach (DatabaseSelectQueryWhere *where, m_wheres)
        where->bindValue(m_query);

    foreach (DatabaseSelectQueryJoin *join, m_joins)
        join->bindValue(m_query);

    if (!m_query->exec()) {
        qDebug() << m_query->lastError().text();

        return false;
    }
    return true;
}

bool DatabaseSelectQuery::first()
{
    if (!m_executed && !execute())
        return false;

    return m_query->first();
}

bool DatabaseSelectQuery::next()
{
    if (!m_executed && !execute())
        return false;

    return m_query->next();
}

QVariant DatabaseSelectQuery::value(const QString &key) const
{
    return m_query->value(m_query->record().indexOf(key));
}

void DatabaseSelectQuery::where(const QString &column, const QVariant &value, IDatabaseSelectQuery::Operator op, bool bind)
{
    DatabaseSelectQueryWhere *w = new DatabaseSelectQueryWhere(column, value, op, bind, this);
    m_wheres.append(w);
}

IDatabaseSelectQueryJoin *DatabaseSelectQuery::join(const QString &table, JoinType joinType)
{
    DatabaseSelectQueryJoin *join = new DatabaseSelectQueryJoin(table, joinType, this);
    m_joins.append(join);
    return join;
}
