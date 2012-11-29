#include "databaseselectqueryjoin.h"

#include "databaseselectquerywhere.h"

DatabaseSelectQueryJoin::DatabaseSelectQueryJoin(const QString &table, IDatabaseSelectQuery::JoinType joinType, QObject *parent) :
    QObject(parent),
    m_table(table),
    m_joinType(joinType)
{
}

void DatabaseSelectQueryJoin::where(const QString &column, const QVariant &value, IDatabaseSelectQuery::Operator op, bool bind)
{
    m_wheres.append(new DatabaseSelectQueryWhere(column, value, op, bind, this));
}

QString DatabaseSelectQueryJoin::toString() const
{
    QString res;
    switch (m_joinType) {
    case IDatabaseSelectQuery::Join:
        res = "JOIN";
        break;
    case IDatabaseSelectQuery::LeftJoin:
        res = "LEFT JOIN";
        break;
    case IDatabaseSelectQuery::RightJoin:
        res = "RIGHT JOIN";
        break;
    }
    res.append(" " + m_table);
    if (m_wheres.count()) {
        res.append(" ON");
        foreach (DatabaseSelectQueryWhere *where, m_wheres) {
            res.append(" " + where->toString());
        }
    }
    return res;
}

void DatabaseSelectQueryJoin::bindValue(QSqlQuery *query)
{
    foreach (DatabaseSelectQueryWhere *where, m_wheres)
        where->bindValue(query);
}
