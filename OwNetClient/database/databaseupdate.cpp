#include "databaseupdate.h"

#include "databasesettings.h"
#include "jsondocument.h"
#include "databaseupdatequery.h"
#include "idatabaseselectquerywheregroup.h"

#include <QDebug>
#include <QSqlQuery>
#include <QStringList>

QMap<int, int> *DatabaseUpdate::m_lastRecordNumbers = new QMap<int, int>;

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
        DatabaseUpdateQuery query("client_sync_records");

        IDatabaseSelectQueryWhereGroup *where = query.whereGroup(IDatabaseSelectQuery::And);
        if (i.key() == -1) {
            where->where("group_id", "NULL", IDatabaseSelectQuery::Is);
        } else {
            where->where("group_id", i.key());
            query.setColumnValue("group_id", i.key());
        }
        where->where("client_id", databaseSettings.clientId());

        query.setColumnValue("client_id", databaseSettings.clientId());
        query.setColumnValue("last_client_rec_num", i.value());

        query.executeQuery();
    }
}

void DatabaseUpdate::setLastRecordNumber(int groupId, int clientRecNum)
{
    if (m_lastRecordNumbers->contains(groupId) && m_lastRecordNumbers->value(groupId) >= clientRecNum)
        return;
    m_lastRecordNumbers->insert(groupId, clientRecNum);
}
