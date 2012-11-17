#include "databaseupdatequery.h"

#include "messagehelper.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>
#include <QDateTime>

DatabaseUpdateQuery::DatabaseUpdateQuery(const QString &table, EntryType type, QObject *parent) :
    QObject(parent), m_needsSave(true)
{
    setTable(table);
    setType(type);
}

DatabaseUpdateQuery::DatabaseUpdateQuery(const QVariantMap &content, QObject *parent)
    : QObject(parent), m_needsSave(false)
{
    m_content = content;
}

void DatabaseUpdateQuery::setTable(const QString &table)
{
    m_content.insert("table", table);
}

void DatabaseUpdateQuery::setType(DatabaseUpdateQuery::EntryType type)
{
    m_content.insert("type", (int)type);
}

void DatabaseUpdateQuery::setReturningId(const QString &useColumnAs, const QVariantList &queries)
{
    QVariantMap returningMap;
    returningMap.insert("use_column_as", useColumnAs);
    returningMap.insert("queries", queries);
    m_content.insert("returning_id", returningMap);
}

void DatabaseUpdateQuery::setColumnValue(const QString &name, const QVariant &value)
{
    m_columns.insert(name, value);
}

void DatabaseUpdateQuery::setTemporaryBinding(const QString &name, const QVariant &value)
{
    m_temporaryBindings.insert(name, value);
}

void DatabaseUpdateQuery::setWhere(const QString &name, const QVariant &value)
{
    m_wheres.insert(name, value);
}

void DatabaseUpdateQuery::setUpdateDates(bool setDates)
{
    m_content.insert("set_update_dates", setDates);
}

QVariant DatabaseUpdateQuery::bindingValue(const QString &name, const QVariant &value)
{
    if (m_temporaryBindings.contains(name))
        return m_temporaryBindings.value(name);
    return value;
}

void DatabaseUpdateQuery::save()
{
    m_content.insert("columns", m_columns);
    m_content.insert("where", m_wheres);
}

bool DatabaseUpdateQuery::executeQuery()
{
    if (m_needsSave)
        save();

    EntryType entryType = type();

    QSqlQuery query;
    QString queryString;
    if (entryType == Detect && m_content.contains("where")) {
        queryString = QString("SELECT 1 FROM %1").arg(table());

        appendWhere(queryString, query);

        if (query.exec() && query.next())
            entryType = Update;
        else
            entryType = Insert;
    }

    bool setDates = m_content.contains("set_update_dates") && m_content.value("set_update_dates").toBool();

    QVariantMap columns = m_content.value("columns").toMap();
    QStringList columnKeys = columns.keys();
    if (entryType == Insert) {
        if (setDates) {
            columnKeys.append("date_created");
            columnKeys.append("date_updated");
        }

        queryString = QString("INSERT INTO %1 (%2) VALUES (:%3)")
                .arg(table())
                .arg(columnKeys.join(", "))
                .arg(columnKeys.join(", :"));

        query.prepare(queryString);
    } else if (entryType == Update) {
        if (setDates)
            columnKeys.append("date_updated");

        queryString = QString("UPDATE %1 SET").arg(table());
        for (int i = 0; i < columnKeys.count(); ++i) {
            if (i > 0)
                queryString.append(",");
            queryString.append(QString(" %1 = :%1").arg(columnKeys.at(i)));
        }

        appendWhere(queryString, query);
    } else if (entryType == Delete) {
        queryString = QString("DELETE FROM %1").arg(table());

        appendWhere(queryString, query);
    } else {
        return false;
    }
    for (int i = 0; i < columnKeys.count(); ++i) {
        if (setDates && (columnKeys.at(i) == "date_created" || columnKeys.at(i) == "date_updated"))
            query.bindValue(":" + columnKeys.at(i),
                            timestamp());
        else
            query.bindValue(":" + columnKeys.at(i),
                            bindingValue(columnKeys.at(i), columns.value(columnKeys.at(i))));
    }

    if (!query.exec()) {
        MessageHelper::debug(query.lastError().text());
        return false;
    } else {
        if (entryType == Insert && m_content.contains("returning_id")) {
            QVariantMap returningMap = m_content.value("returning_id").toMap();
            QVariant value = query.lastInsertId();
            QVariantList queries = returningMap.value("queries").toList();

            for (int i = 0; i < queries.count(); ++i) {
                DatabaseUpdateQuery updateQuery(queries.at(i).toMap(), this);
                updateQuery.setTemporaryBinding(returningMap.value("use_column_as").toString(), value);
                updateQuery.executeQuery();
            }
        }
    }
    return true;
}

QString DatabaseUpdateQuery::table() const
{
    return m_content.value("table").toString();
}

DatabaseUpdateQuery::EntryType DatabaseUpdateQuery::type() const
{
    return (EntryType) m_content.value("type").toInt();
}

QVariantMap DatabaseUpdateQuery::content() const
{
    return m_content;
}

QString DatabaseUpdateQuery::timestamp() const
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

void DatabaseUpdateQuery::appendWhere(QString &queryString, QSqlQuery &query)
{
    QVariantMap where = m_content.value("where").toMap();
    if (!where.count()) {
        query.prepare(queryString);
        return;
    }

    queryString.append(" WHERE");
    QStringList whereKeys = where.keys();

    for (int i = 0; i < whereKeys.count(); ++i) {
        if (i > 0)
            queryString.append(" AND");
        queryString.append(QString(" %1 = :%1").arg(whereKeys.at(i)));
    }
    query.prepare(queryString);

    for (int i = 0; i < whereKeys.count(); ++i)
        query.bindValue(":" + whereKeys.at(i),
                        bindingValue(whereKeys.at(i), where.value(whereKeys.at(i))));
}
