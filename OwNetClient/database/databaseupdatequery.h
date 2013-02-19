#ifndef DATABASEUPDATEQUERY_H
#define DATABASEUPDATEQUERY_H

#include "idatabaseupdatequery.h"
#include "idatabaseselectquery.h"

#include <QObject>
#include <QVariantMap>

class QSqlQuery;
class DatabaseSelectQuery;

/**
 * @brief Database query to update, insert or delete data.
 * Can be exported to JSON.
 */
class DatabaseUpdateQuery : public QObject, public IDatabaseUpdateQuery
{
    Q_OBJECT
public:
    DatabaseUpdateQuery(const QString &table, EntryType type = InsertOrUpdate, QObject *parent = 0);

    void setTable(const QString &table);
    void setType(EntryType type);
    void setColumnValue(const QString &name, const QVariant &value);
    void setUpdateDates(bool setDates);
    void setUpdateDates(IDatabaseUpdateQuery::UpdateDates updateDates);
    bool executeQuery();

    void singleWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equal, bool bind = true);
    IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op);

    QString table() const;
    EntryType type() const;

protected:
    virtual bool executeInsert();
    virtual bool executeUpdate();
    virtual bool executeDelete();
    bool executeQuery(const QString &queryString) const;
    bool executeQuery(QSqlQuery &query) const;
    QString timestamp() const;

    QVariantMap m_columns;
    DatabaseSelectQuery *m_selectQuery;

private:
    QString m_table;
    DatabaseUpdateQuery::EntryType m_type;
    QVariantMap m_temporaryBindings;
};

#endif // DATABASEUPDATEQUERY_H
