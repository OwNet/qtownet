#include "databaseupdate.h"

#include "databasesettings.h"
#include "jsondocument.h"

#include <QDebug>
#include <QSqlQuery>
#include <QStringList>

QMap<int, int> *DatabaseUpdate::m_lastRecordNumbers = new QMap<int, int>;

DatabaseUpdate::DatabaseUpdate(QObject *parent)
    : QObject(parent), m_sync(true), m_syncWith(-1), m_groupId(-1)
{
}

/**
 * @brief Create a new DatabaseUpdate object
 * @param sync True if the query should be saved in journal
 * @param parent
 */
DatabaseUpdate::DatabaseUpdate(bool sync, QObject *parent)
    : QObject(parent), m_sync(sync), m_syncWith(-1), m_groupId(-1)
{
}

/**
 * @brief Construct a DatabaseUpdate object from a JSON representation of the queries
 * @param content
 * @param sync
 * @param parent
 */
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
 * @brief Save record numbers of the last created queries of the client in sync journal to client_sync_records.
 *
 * Called periodically.
 */
void DatabaseUpdate::saveLastRecordNumbers()
{
    QMap<int, int> recordNumbers(*m_lastRecordNumbers);
    m_lastRecordNumbers->clear();
    DatabaseSettings databaseSettings;

    for (auto i = recordNumbers.begin(); i != recordNumbers.end(); ++i) {
        QString where;
        if (i.key() == -1)
            where += "group_id IS NULL";
        else
            where += QString("group_id = %1").arg(i.key());
        where += QString(" AND client_id = %1").arg(databaseSettings.clientId());

        QSqlQuery query;
        query.prepare(QString("SELECT 1 FROM client_sync_records WHERE %1").arg(where));
        if (query.exec() && query.next()) {
            QSqlQuery update;
            update.prepare(QString("UPDATE client_sync_records SET last_client_rec_num = %1 WHERE %2")
                           .arg(i.value())
                           .arg(where));
            update.exec();
        } else {
            QStringList columns;
            QStringList values;
            if (i.key() != -1) {
                columns.append("group_id");
                values.append(QString::number(i.key()));
            }
            columns.append("client_id");
            values.append(QString::number(databaseSettings.clientId()));

            columns.append("last_client_rec_num");
            values.append(QString::number(i.value()));

            QSqlQuery insert;
            insert.prepare(QString("INSERT INTO client_sync_records (%1) VALUES (%2)")
                    .arg(columns.join(", "))
                    .arg(values.join(", ")));
            insert.exec();
        }
    }
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

    DatabaseUpdate update(false);
    IDatabaseUpdateQuery *query = update.createUpdateQuery("sync_journal", IDatabaseUpdateQuery::Insert);
    query->setUpdateDates(IDatabaseUpdateQuery::DateCreated);

    DatabaseSettings databaseSettings;
    int clientRecNum = databaseSettings.nextClientSyncRecordNumber();
    query->setColumnValue("client_id", databaseSettings.clientId());
    query->setColumnValue("client_rec_num", clientRecNum);
    query->setColumnValue("content", JsonDocument::fromVariantList(content).toJson());
    if (m_syncWith != -1)
        query->setColumnValue("sync_with", m_syncWith);
    if (m_groupId != -1)
        query->setColumnValue("group_id", m_groupId);

    m_lastRecordNumbers->insert(m_groupId, clientRecNum);

    update.execute();
}
