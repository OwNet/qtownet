#include "databaseupdate.h"

#include "databasesettings.h"
#include "qjson/serializer.h"

#include <QDebug>

DatabaseUpdate::DatabaseUpdate(QObject *parent)
    : QObject(parent), m_sync(true), m_syncWith(-1), m_groupId(-1)
{
}

DatabaseUpdate::DatabaseUpdate(bool sync, QObject *parent)
    : QObject(parent), m_sync(sync), m_syncWith(-1), m_groupId(-1)
{
}

DatabaseUpdate::DatabaseUpdate(const QVariantList &content, bool sync, QObject *parent)
    : QObject(parent), m_sync(sync), m_syncWith(-1), m_groupId(-1)
{
    foreach (QVariant queryContent, content)
        createUpdateQuery(queryContent.toMap());
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
 * @return New IDatabaseUpdateQuery
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
    foreach (DatabaseUpdateQuery *query, m_updateQueries)
        if (!query->executeQuery())
            numFailed++;

    if (!numFailed && m_sync)
        saveToJournal();

    return numFailed;
}

/**
 * @brief Save queries to sync journal
 */
void DatabaseUpdate::saveToJournal()
{
    if (!m_updateQueries.count())
        return;

    QVariantList content;
    foreach (IDatabaseUpdateQuery *query, m_updateQueries)
        content.append(query->content());
    QJson::Serializer serializer;

    DatabaseUpdate update(false);
    IDatabaseUpdateQuery *query = update.createUpdateQuery("sync_journal", IDatabaseUpdateQuery::Insert);
    query->setUpdateDates(IDatabaseUpdateQuery::DateCreated);

    query->setColumnValue("client_id", DatabaseSettings().clientId());
    query->setColumnValue("client_rec_num", DatabaseSettings().nextClientSyncRecordNumber());
    query->setColumnValue("content", serializer.serialize(content));
    if (m_syncWith != -1)
        query->setColumnValue("sync_with", m_syncWith);
    if (m_groupId != -1)
        query->setColumnValue("group_id", m_groupId);

    update.execute();
}
