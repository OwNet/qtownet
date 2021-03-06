#ifndef DATABASESELECTQUERYJOIN_H
#define DATABASESELECTQUERYJOIN_H

#include <QObject>

#include "idatabaseselectqueryjoin.h"

class IDatabaseSelectQueryWhere;
class QSqlQuery;

/**
 * @brief Enables creating JOIN expressions in DatabaseSelectQuery.
 *
 * Join expressions can have complex ON clauses, created by singleWhere. or whereGroup().
 * Each call to those functions overwrites the previous calls.
 */
class DatabaseSelectQueryJoin : public QObject, public IDatabaseSelectQueryJoin
{
    Q_OBJECT
public:
    explicit DatabaseSelectQueryJoin(const QString &table, IDatabaseSelectQuery::JoinType joinType = IDatabaseSelectQuery::Join, QObject *parent = 0);

    void singleWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equal, bool bind = true);
    IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op);

    QString toString() const;

    void bindValue(QSqlQuery *query);
    
private:
    QString m_table;
    IDatabaseSelectQueryWhere *m_where;
    IDatabaseSelectQuery::JoinType m_joinType;
};

#endif // DATABASESELECTQUERYJOIN_H
