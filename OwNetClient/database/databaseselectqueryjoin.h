#ifndef DATABASESELECTQUERYJOIN_H
#define DATABASESELECTQUERYJOIN_H

#include <QObject>

#include "idatabaseselectqueryjoin.h"

class DatabaseSelectQueryWhere;
class QSqlQuery;

class DatabaseSelectQueryJoin : public QObject, public IDatabaseSelectQueryJoin
{
    Q_OBJECT
public:
    explicit DatabaseSelectQueryJoin(const QString &table, IDatabaseSelectQuery::JoinType joinType = IDatabaseSelectQuery::Join, QObject *parent = 0);

    void where(const QString &column, const QVariant &value, IDatabaseSelectQuery::Operator op = IDatabaseSelectQuery::Equals, bool bind = true);

    QString toString() const;

    void bindValue(QSqlQuery *query);
    
private:
    QString m_table;
    QList<DatabaseSelectQueryWhere *> m_wheres;
    IDatabaseSelectQuery::JoinType m_joinType;
};

#endif // DATABASESELECTQUERYJOIN_H
