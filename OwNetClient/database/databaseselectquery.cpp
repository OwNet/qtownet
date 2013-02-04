#include "databaseselectquery.h"

#include "databaseselectqueryjoin.h"
#include "idatabaseselectquerywhere.h"
#include "databaseselectquerywheregroup.h"
#include "databaseselectquerywhereexpression.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

/**
 * @brief The only possible constructor
 * @param table Name of the table
 * @param parent QObject parent
 */
DatabaseSelectQuery::DatabaseSelectQuery(const QString &table, QObject *parent) :
    QObject(parent),
    m_table(table),
    m_limit(-1),
    m_offset(-1),
    m_executed(false),
    m_where(NULL)
{
    m_query = new QSqlQuery;
}

DatabaseSelectQuery::~DatabaseSelectQuery()
{
    delete m_query;
}

/**
 * @brief Constructs and executes the select query.
 * Called by first() and next()
 * @return True if successful
 */
bool DatabaseSelectQuery::execute()
{
    m_executed = true;

    QString queryString = QString("SELECT %1 FROM %2")
            .arg(m_selectColumns.count() ? m_selectColumns.join(", ") : "*")
            .arg(m_table);

    foreach (DatabaseSelectQueryJoin *join, m_joins) {
        queryString.append(" " + join->toString());
    }

    if (m_where)
        queryString.append(QString(" WHERE %1").arg(m_where->toString()));

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

    if (m_where)
        m_where->bindValue(m_query);

    foreach (DatabaseSelectQueryJoin *join, m_joins)
        join->bindValue(m_query);

    qDebug() << queryString;
    if (!m_query->exec()) {
        qDebug() << m_query->lastError().text();

        return false;
    }
    return true;
}

/**
 * @brief First result from the query results.
 * Executes the query if not executed yet.
 * @return True if successful
 */
bool DatabaseSelectQuery::first()
{
    if (!m_executed && !execute())
        return false;

    return m_query->first();
}

/**
 * @brief Retrieves the next record from the query results.
 * Executes the query if not executed yet.
 * @return True if successful
 */
bool DatabaseSelectQuery::next()
{
    if (!m_executed && !execute())
        return false;

    return m_query->next();
}

/**
 * @brief Value for an attributed of the executed query.
 * Query needs to be executed first by running next() or first()
 * @param key Name of the attribute
 * @return Value for the attribute
 */
QVariant DatabaseSelectQuery::value(const QString &key) const
{
    return m_query->value(m_query->record().indexOf(key));
}

/**
 * @brief Inserts a single where clause. Careful, this overwrites any existing where clauses.
 * @param column Name of the attribute
 * @param value Value of the attribute
 * @param op Type of compare operation
 * @param bind True if the value should be escaped
 */
void DatabaseSelectQuery::singleWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op, bool bind)
{
    m_where = new DatabaseSelectQueryWhereExpression(column, value, op, bind, this);
}

/**
 * @brief Creates a DatabaseSelectQueryWhereGroup object which supports multiple where clauses.
 * Overwrites any existing where clauses.
 * @param op Type of operation
 * @return
 */
IDatabaseSelectQueryWhereGroup *DatabaseSelectQuery::whereGroup(IDatabaseSelectQuery::JoinOperator op)
{
    DatabaseSelectQueryWhereGroup *group = new DatabaseSelectQueryWhereGroup(op, this);
    m_where = group;
    return group;
}

/**
 * @brief Creates a new IDatabaseSelectQueryJoin object to create a join expression.
 * Appends the object to the list of joins.
 * @param table Then name of the table to join
 * @param joinType Type of join expression
 * @return IDatabaseSelectQueryJoin join object
 */
IDatabaseSelectQueryJoin *DatabaseSelectQuery::join(const QString &table, JoinType joinType)
{
    DatabaseSelectQueryJoin *join = new DatabaseSelectQueryJoin(table, joinType, this);
    m_joins.append(join);
    return join;
}

void DatabaseSelectQuery::resetQuery()
{
    if (m_query)
        delete m_query;
    m_query = new QSqlQuery;
    m_executed = false;
}
