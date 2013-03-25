#include "cleansyncjournaljob.h"

#include "iproxyconnection.h"
#include "idatabaseupdatequery.h"
#include "isynceddatabaseupdatequery.h"
#include "idatabaseselectquerywheregroup.h"

CleanSyncJournalJob::CleanSyncJournalJob(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void CleanSyncJournalJob::execute()
{
    if (!m_cleanMutex.tryLock())
        return;

    QObject parent;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("sync_journal", &parent, false);
    query->setType(IDatabaseUpdateQuery::Delete);
    IDatabaseSelectQueryWhereGroup *where = query->whereGroup(IDatabaseSelectQuery::And);
    where->where("group_id", ISyncedDatabaseUpdateQuery::GroupCaches);
    where->where("id",
                       QString("(SELECT id FROM sync_journal WHERE group_id = %1 ORDER BY date_created DESC, id DESC LIMIT %2)")
                        .arg(ISyncedDatabaseUpdateQuery::GroupCaches)
                        .arg(MaxNumberOfCacheItems),
                       IDatabaseSelectQuery::NotIn,
                       false);
    if (!query->executeQuery())
        m_proxyConnection->message(tr("Failed to remove old cache items from sync journal."));

    m_cleanMutex.unlock();
}
