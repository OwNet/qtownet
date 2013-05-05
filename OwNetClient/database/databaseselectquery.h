#ifndef DATABASESELECTQUERY_H
#define DATABASESELECTQUERY_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

#include "idatabaseselectquery.h"

class QSqlQuery;
class IDatabaseSelectQueryWhere;
class DatabaseSelectQueryJoin;
class IDatabaseSelectQueryWhereGroup;

/**
 * @brief Wrapper for database SELECT queries.
 *
 * Constructs and executes the query when next() or first() is called.
 * Enables retrieving values for attributes from the results of the executed query using value(attribute).
 *
 * Supports complex where expressions. Beware, there can only be a single root where expression
 * created by either singleWhere() or whereGroup(). Each call overwrites the previous expression.
 */
class DatabaseSelectQuery : public QObject, public IDatabaseSelectQuery
{
    Q_OBJECT
public:
    explicit DatabaseSelectQuery(const QString &table, QObject *parent = 0);
    ~DatabaseSelectQuery();

    bool first();
    bool next();
    QVariant value(const QString &key) const;

    void select(const QString &column) { m_selectColumns.append(column); }
    void select(const QStringList &columns) { m_selectColumns.append(columns); }
    void clearSelect() { m_selectColumns.clear(); }

    void singleWhere(const QString &column, const QVariant &value, WhereOperator op = Equal, bool bind = true);
    IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op = IDatabaseSelectQuery::And);

    IDatabaseSelectQueryJoin *join(const QString &table, IDatabaseSelectQuery::JoinType joinType = Join);

    void groupBy(const QString &column) { m_groupByColumns.append(column); }
    void groupBy(const QStringList &columns) { m_groupByColumns.append(columns); }

    void orderBy(const QString &column, IDatabaseSelectQuery::Order order = IDatabaseSelectQuery::Ascending);

    void limit(int l) { m_limit = l; }
    void offset(int o) { m_offset = o; }

    IDatabaseSelectQueryWhere *currentWhereQuery() { return m_where; }

    QSqlQuery *query() { return m_query; }
    void resetQuery();

    int numberOfPages(int numItemsPerPage);
    void page(int page, int numItemsPerPage);

private:
    bool execute();

    QString m_table;
    QStringList m_selectColumns;
    QStringList m_groupByColumns;
    QList<QPair<QString, IDatabaseSelectQuery::Order> > m_orderByColumns;
    QList<DatabaseSelectQueryJoin *> m_joins;
    IDatabaseSelectQueryWhere *m_where;
    QSqlQuery *m_query;
    int m_limit;
    int m_offset;
    bool m_executed;
};

#endif // DATABASESELECTQUERY_H
