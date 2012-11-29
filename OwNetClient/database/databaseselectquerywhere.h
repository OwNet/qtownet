#ifndef DATABASESELECTQUERYWHERE_H
#define DATABASESELECTQUERYWHERE_H

#include <QObject>

#include "idatabaseselectquery.h"

class QSqlQuery;

class DatabaseSelectQueryWhere : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseSelectQueryWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::Operator op = IDatabaseSelectQuery::Equals, bool bind = true, QObject *parent = 0);
    
    QString toString() const;
    void bindValue(QSqlQuery *query) const;

private:
    QString operatorToString() const;

    QString m_column;
    QVariant m_value;
    IDatabaseSelectQuery::Operator m_operator;
    int m_id;
    bool m_bind;
    static int m_nextId;
};

#endif // DATABASESELECTQUERYWHERE_H
