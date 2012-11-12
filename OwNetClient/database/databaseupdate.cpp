#include "databaseupdate.h"

#include <QDebug>
#include "qjson/serializer.h"

DatabaseUpdate::DatabaseUpdate(bool sync, QObject *parent) :
    QObject(parent), m_sync(sync)
{
}

DatabaseUpdateQuery *DatabaseUpdate::createUpdateQuery(const QString &table, DatabaseUpdateQuery::EntryType type)
{
    DatabaseUpdateQuery *query = new DatabaseUpdateQuery(table, type, this);
    m_updateQueries.append(query);
    return query;
}

void DatabaseUpdate::execute()
{
    for (int i = 0; i < m_updateQueries.count(); ++i) {
        m_updateQueries.at(i)->executeQuery();
    }
    // TODO if m_sync, save to sync journal
}
