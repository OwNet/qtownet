#include "databaseselectqueryjoin.h"

#include "databaseselectquerywhereexpression.h"
#include "databaseselectquerywheregroup.h"

DatabaseSelectQueryJoin::DatabaseSelectQueryJoin(const QString &table, IDatabaseSelectQuery::JoinType joinType, QObject *parent) :
    QObject(parent),
    m_table(table),
    m_joinType(joinType),
    m_where(NULL)
{
}

void DatabaseSelectQueryJoin::singleWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op, bool bind)
{
    m_where = new DatabaseSelectQueryWhereExpression(column, value, op, bind, this);
}

IDatabaseSelectQueryWhereGroup *DatabaseSelectQueryJoin::whereGroup(IDatabaseSelectQuery::JoinOperator op)
{
    DatabaseSelectQueryWhereGroup *group = new DatabaseSelectQueryWhereGroup(op, this);
    m_where = group;
    return group;
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
    if (m_where) {
        res.append(QString(" ON %1").arg(m_where->toString()));
    }
    return res;
}

void DatabaseSelectQueryJoin::bindValue(QSqlQuery *query)
{
    if (m_where)
        m_where->bindValue(query);
}
