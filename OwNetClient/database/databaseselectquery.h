#ifndef DATABASESELECTQUERY_H
#define DATABASESELECTQUERY_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

#include "idatabaseselectquery.h"

class QSqlQuery;
class DatabaseSelectQueryWhere;
class DatabaseSelectQueryJoin;

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

    void where(const QString &column, const QVariant &value, Operator op = Equals, bool bind = true);

    IDatabaseSelectQueryJoin *join(const QString &table, IDatabaseSelectQuery::JoinType joinType = Join);

    void groupBy(const QString &column) { m_groupByColumns.append(column); }
    void groupBy(const QStringList &columns) { m_groupByColumns.append(columns); }

    void orderBy(const QString &column) { m_orderByColumns.append(column); }
    void orderBy(const QStringList &columns) { m_orderByColumns.append(columns); }

    void limit(int l) { m_limit = l; }
    void offset(int o) { m_offset = o; }
    
private:
    bool execute();

    QString m_table;
    QStringList m_selectColumns;
    QStringList m_groupByColumns;
    QStringList m_orderByColumns;
    QList<DatabaseSelectQueryJoin *> m_joins;
    QList<DatabaseSelectQueryWhere *> m_wheres;
    QSqlQuery *m_query;
    int m_limit;
    int m_offset;
    bool m_executed;
};

#endif // DATABASESELECTQUERY_H
