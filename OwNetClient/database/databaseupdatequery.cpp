#include "databaseupdatequery.h"

#include "messagehelper.h"
#include "databaseselectquery.h"
#include "databasesettings.h"
#include "databaseupdate.h"
#include "idatabaseselectquerywhere.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QDateTime>

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
 * @brief Set the name of the table
 * @param table Name of the table
 */
void DatabaseUpdateQuery::setTable(const QString &table)
{
    m_table = table;
}

/**
 * @brief Set the type of the query, i.e. update, insert, delete
 * @param type Type of the query
 */
void DatabaseUpdateQuery::setType(DatabaseUpdateQuery::EntryType type)
{
    m_type = type;
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
 * @brief Execute the query
 * @return True if successful
 */
bool DatabaseUpdateQuery::executeQuery()
{
    if (type() == Delete)
        return executeDelete();

    if (type() == InsertOrUpdate) {
        if (m_selectQuery) {
            m_selectQuery->select("1");
            if (m_selectQuery->first())
                return executeUpdate();
        }

        return executeInsert();
    }
    return false;
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
 * @return Name of the table
 */
QString DatabaseUpdateQuery::table() const
{
    return m_table;
}

/**
 * @return Type of the query - insert, update, delete
 */
DatabaseUpdateQuery::EntryType DatabaseUpdateQuery::type() const
{
    return m_type;
}

/**
 * @brief Current datetime to be used in date_updated or date_created
 */
QString DatabaseUpdateQuery::timestamp() const
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}
