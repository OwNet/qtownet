#ifndef DATABASEUPDATEQUERY_H
#define DATABASEUPDATEQUERY_H

#include "idatabaseupdatequery.h"
#include "idatabaseselectquery.h"

#include <QObject>
#include <QVariantMap>

class QSqlQuery;
class DatabaseSelectQuery;
class IDatabaseUpdateListener;

/**
 * @brief Database query to update, insert or delete data.
 */
class DatabaseUpdateQuery : public QObject, public IDatabaseUpdateQuery
{
    Q_OBJECT
public:
    enum ForceOperation {
        DontForce = 0,
        ForceInsert = 1,
        ForceUpdate = 2
    };

    DatabaseUpdateQuery(const QString &table, EntryType type = InsertOrUpdate, QObject *parent = 0);

    void setTable(const QString &table) { m_table = table; }
    void setType(EntryType type) { m_type = type; }
    void setColumnValue(const QString &name, const QVariant &value);
    void setUpdateDates(bool setDates);
    void setUpdateDates(IDatabaseUpdateQuery::UpdateDates updateDates);
    bool executeQuery();

    void singleWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op = IDatabaseSelectQuery::Equal, bool bind = true);
    IDatabaseSelectQueryWhereGroup *whereGroup(IDatabaseSelectQuery::JoinOperator op);

    QString table() const { return m_table; }
    EntryType type() const { return m_type; }
    QVariantList columnsForListeners() const { return m_columnsForListeners; }

    static void registerListener(IDatabaseUpdateListener *listener);
    static void deregisterListener(IDatabaseUpdateListener *listener);

    void setForceOperation(ForceOperation force) { m_forceOperation = force; }

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
    void saveColumnsForListeners();

    QString m_table;
    DatabaseUpdateQuery::EntryType m_type;
    QVariantMap m_temporaryBindings;
    QVariantList m_columnsForListeners;
    ForceOperation m_forceOperation;
    static QMap<QString, QList<IDatabaseUpdateListener*>* > *m_listeners;
};

#endif // DATABASEUPDATEQUERY_H
