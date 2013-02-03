#ifndef IDATABASEUPDATEQUERY_H
#define IDATABASEUPDATEQUERY_H

#include <QString>
#include <QVariant>

#include "idatabaseselectquery.h"

class IDatabaseUpdateQuery
{
public:
    enum EntryType {
        InsertOrUpdate = 1,
        Delete = 4
    };
    enum UpdateDates {
        None = 0,
        DateCreated = 1,
        DateUpdated = 2
    };

    virtual void setColumnValue(const QString &name, const QVariant &value) = 0;
    virtual void setUpdateDates(bool setDates) = 0;
    virtual void setUpdateDates(IDatabaseUpdateQuery::UpdateDates updateDates) = 0;

    virtual void singleWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equal, bool bind = true) = 0;
    virtual IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op) = 0;

    virtual QString table() const = 0;
    virtual EntryType type() const = 0;
    virtual void setType(EntryType type) = 0;

    virtual bool executeQuery() = 0;
};

#endif // IDATABASEUPDATEQUERY_H
