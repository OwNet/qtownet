#include "databaseupdatequery.h"

#include "messagehelper.h"
#include "databaseselectquery.h"
#include "databasesettings.h"
#include "databaseupdate.h"
#include "idatabaseselectquerywhere.h"
#include "idatabaseupdatelistener.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QDateTime>

QMap<QString, QList<IDatabaseUpdateListener*>* > *DatabaseUpdateQuery::m_listeners = new QMap<QString, QList<IDatabaseUpdateListener*>* >();

DatabaseUpdateQuery::DatabaseUpdateQuery(const QString &table, EntryType type, QObject *parent) :
    QObject(parent),
    m_selectQuery(NULL),
    m_table(table),
    m_type(type)
{
}

bool DatabaseUpdateQuery::executeInsert()
{
    QStringList columnKeys = m_columns.keys();

    return executeQuery(QString("INSERT INTO %1 (%2) VALUES (:v_%3)")
                        .arg(table())
                        .arg(columnKeys.join(", "))
                        .arg(columnKeys.join(", :v_")));
}

bool DatabaseUpdateQuery::executeUpdate()
{
    QStringList columnKeys = m_columns.keys();

    QString queryString = QString("UPDATE %1 SET").arg(table());

    for (int i = 0; i < columnKeys.count(); ++i) {
        if (i > 0)
            queryString.append(",");
        queryString.append(QString(" %1 = :v_%1").arg(columnKeys.at(i)));
    }
    queryString.append(QString(" WHERE %1").arg(m_selectQuery->currentWhereQuery()->toString()));

    QSqlQuery query;
    query.prepare(queryString);
    m_selectQuery->currentWhereQuery()->bindValue(&query);

    return executeQuery(query);
}

bool DatabaseUpdateQuery::executeDelete()
{
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE %2")
                  .arg(table())
                  .arg(m_selectQuery->currentWhereQuery()->toString()));
    m_selectQuery->currentWhereQuery()->bindValue(&query);

    return executeQuery(query);
}

bool DatabaseUpdateQuery::executeQuery(const QString &queryString) const
{
    QSqlQuery query;
    query.prepare(queryString);
    return executeQuery(query);
}

bool DatabaseUpdateQuery::executeQuery(QSqlQuery &query) const
{
    QStringList columnKeys = m_columns.keys();
    foreach (QString key, columnKeys)
        query.bindValue(":v_" + key, m_columns.value(key));

    if (!query.exec()) {
        MessageHelper::debug(query.lastQuery());
        MessageHelper::debug(query.lastError().text());
        return false;
    }

    return true;
}

/**
 * @brief Set the values for the column
 * @param name Name of the column
 * @param value Value of the column
 */
void DatabaseUpdateQuery::setColumnValue(const QString &name, const QVariant &value)
{
    m_columns.insert(name, value);
}

/**
 * @brief Automatically assign date_updated and date_created values if true.
 * @param setDates
 */
void DatabaseUpdateQuery::setUpdateDates(bool setDates)
{
    if (setDates) {
        m_columns.insert("date_created", timestamp());
        m_columns.insert("date_updated", timestamp());
    }
}

/**
 * @brief Automatically insert current dates for the given columns
 * @param updateDates Columns to be used
 */
void DatabaseUpdateQuery::setUpdateDates(IDatabaseUpdateQuery::UpdateDates updateDates)
{
    if (updateDates == DateCreated)
        m_columns.insert("date_created", timestamp());
    if (updateDates == DateUpdated)
        m_columns.insert("date_updated", timestamp());
}


/**
 * @brief Executes the query and notify the listeners for the table
 * @return True if successful
 */
bool DatabaseUpdateQuery::executeQuery()
{
    bool success = false;
    if (type() == Delete) {
        success = executeDelete();
    } else if (type() == InsertOrUpdate) {
        bool update = false;
        if (m_selectQuery) {
            m_selectQuery->select("1");
            update = m_selectQuery->first();
        }

        if (update)
            success = executeUpdate();
        else
            success = executeInsert();
    }

    /// Notify the listeners
    if (success)
        if (m_listeners->contains(table()))
            foreach (IDatabaseUpdateListener *listener, *(m_listeners->value(table())))
                listener->tableUpdated(this);

    return success;
}

/**
 * @brief Inserts a single where clause. Careful, this overwrites any existing where clauses.
 * @param column Name of the attribute
 * @param value Value of the attribute
 * @param op Type of compare operation
 * @param bind True if the value should be escaped
 */
void DatabaseUpdateQuery::singleWhere(const QString &column, const QVariant &value, IDatabaseSelectQuery::WhereOperator op, bool bind)
{
    if (!m_selectQuery)
        m_selectQuery = new DatabaseSelectQuery(m_table);
    m_selectQuery->singleWhere(column, value, op, bind);
}

/**
 * @brief Creates a DatabaseSelectQueryWhereGroup object which supports multiple where clauses.
 * Overwrites any existing where clauses.
 * @param op Type of operation
 * @return
 */
IDatabaseSelectQueryWhereGroup *DatabaseUpdateQuery::whereGroup(IDatabaseSelectQuery::JoinOperator op)
{
    if (!m_selectQuery)
        m_selectQuery = new DatabaseSelectQuery(m_table);
    return m_selectQuery->whereGroup(op);
}

/**
 * @brief Add a new listener that will be notified when the listened table is updated
 * @param listener Listener object
 */
void DatabaseUpdateQuery::registerListener(IDatabaseUpdateListener *listener)
{
    if (m_listeners->contains(listener->tableName())) {
        (*m_listeners)[listener->tableName()]->append(listener);
    } else {
        QList<IDatabaseUpdateListener *> *list = new QList<IDatabaseUpdateListener*>();
        list->append(listener);
        m_listeners->insert(listener->tableName(), list);
    }
}

void DatabaseUpdateQuery::deregisterListener(IDatabaseUpdateListener *listener)
{
    if (m_listeners->contains(listener->tableName()))
        (*m_listeners)[listener->tableName()]->removeAll(listener);
}

/**
 * @brief Current datetime to be used in date_updated or date_created
 */
QString DatabaseUpdateQuery::timestamp() const
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}
