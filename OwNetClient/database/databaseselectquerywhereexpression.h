#ifndef DATABASESELECTQUERYWHEREEXPRESSION_H
#define DATABASESELECTQUERYWHEREEXPRESSION_H

#include <QObject>

#include "idatabaseselectquery.h"
#include "idatabaseselectquerywhere.h"

/**
 * @brief WHERE expression used by DatabaseSelectQuery.
 *
 * Supports just a single key - value expression with custom operation.
 */
class DatabaseSelectQueryWhereExpression : public IDatabaseSelectQueryWhere
{
public:
    explicit DatabaseSelectQueryWhereExpression(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equal, bool bind = true, QObject *parent = 0);
    
    QString toString();
    void bindValue(QSqlQuery *query) const;

private:
    QString operatorToString() const;

    QString m_column;
    QVariant m_value;
    IDatabaseSelectQuery::WhereOperator m_operator;
    int m_id;
    bool m_bind;
    static int m_nextId;
};

#endif // DATABASESELECTQUERYWHEREEXPRESSION_H
