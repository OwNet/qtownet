#ifndef DATABASESELECTQUERYWHERE_H
#define DATABASESELECTQUERYWHERE_H

#include <QObject>

#include "idatabaseselectquery.h"
#include "idatabaseselectquerywhere.h"

class DatabaseSelectQueryWhereExpression : public IDatabaseSelectQueryWhere
{
public:
    explicit DatabaseSelectQueryWhereExpression(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equals, bool bind = true, QObject *parent = 0);
    
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

#endif // DATABASESELECTQUERYWHERE_H
