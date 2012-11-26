#include "databaseupdate.h"

#include <QDebug>
#include "qjson/serializer.h"

DatabaseUpdate::DatabaseUpdate(QObject *parent)
    : QObject(parent), m_sync(true)
{
}

DatabaseUpdate::DatabaseUpdate(bool sync, QObject *parent)
    : QObject(parent), m_sync(sync)
{
}

/**
 * @brief Create a new query
 * @param table Name of the table
 * @param type Query type
 * @return
 */
IDatabaseUpdateQuery *DatabaseUpdate::createUpdateQuery(const QString &table, DatabaseUpdateQuery::EntryType type)
{
    DatabaseUpdateQuery *query = new DatabaseUpdateQuery(table, type, this);
    m_updateQueries.append(query);
    return query;
}

/**
 * @brief Create a new query from QVariantMap
 * @param content JSON formatted content
 * @return
 */
IDatabaseUpdateQuery *DatabaseUpdate::createUpdateQuery(const QVariantMap &content)
{
    DatabaseUpdateQuery *query = new DatabaseUpdateQuery(content, this);
    m_updateQueries.append(query);
    return query;
}

/**
 * @brief Execute queries
 * @return Number of failed queries
 */
int DatabaseUpdate::execute()
{
    int numFailed = 0;
    for (int i = 0; i < m_updateQueries.count(); ++i) {
        if (!m_updateQueries.at(i)->executeQuery())
            numFailed++;
    }

    // TODO if m_sync, save to sync journal

    return numFailed;
}
