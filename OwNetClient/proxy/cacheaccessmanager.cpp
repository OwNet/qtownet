#include "cacheaccessmanager.h"

#include "databaseselectquery.h"
#include "webdownloadsmanager.h"
#include "databaseupdatequery.h"
#include "gdsfclock.h"

#include <QMutexLocker>

CacheAccessManager::CacheAccessManager(QObject *parent) :
    QObject(parent)
{
    init();
}

void CacheAccessManager::logCacheAccess(uint cacheId, qint64 size, int numAccesses)
{
    QMutexLocker locker(&m_mutex);

    CacheInfo info;
    info.numAccesses = 0;
    if (m_caches.contains(cacheId)) {
        info = m_caches[cacheId];
        if (!m_cachesToInsert.contains(cacheId))
            m_cachesToUpdate.insert(cacheId);
    } else {
        m_cachesToInsert.insert(cacheId);
    }

    info.numAccesses += numAccesses;
    if (size >= 0)
        info.size = size;
    info.accessValue = WebDownloadsManager::instance()->gdsfClock()->getGDSFPriority(info.numAccesses, size);

    m_caches.insert(cacheId, info);
}

void CacheAccessManager::saveToDatabase()
{
    m_mutex.lock();
    QSet<uint> cachesToUpdate = m_cachesToUpdate;
    QSet<uint> cachesToInsert = m_cachesToInsert;
    m_cachesToUpdate.clear();
    m_cachesToInsert.clear();
    m_mutex.unlock();

    foreach (uint cacheId, cachesToInsert) {
        CacheInfo info = m_caches.value(cacheId);
        DatabaseUpdateQuery query("caches");
        query.setColumnValue("id", cacheId);
        query.setColumnValue("access_count", info.numAccesses);
        query.setColumnValue("access_value", info.accessValue);
        query.setColumnValue("size", info.size);
        query.setForceOperation(DatabaseUpdateQuery::ForceInsert);
        query.executeQuery();
    }
    foreach (uint cacheId, cachesToUpdate) {
        CacheInfo info = m_caches.value(cacheId);
        DatabaseUpdateQuery query("caches");
        query.setColumnValue("access_count", info.numAccesses);
        query.setColumnValue("access_value", info.accessValue);
        query.setColumnValue("size", info.size);
        query.singleWhere("id", cacheId);
        query.setForceOperation(DatabaseUpdateQuery::ForceUpdate);
        query.executeQuery();
    }
}

void CacheAccessManager::init()
{
    DatabaseSelectQuery selectQuery("caches");
    while (selectQuery.next()) {
        CacheInfo info;
        info.accessValue = selectQuery.value("access_value").toDouble();
        info.numAccesses = selectQuery.value("access_count").toInt();
        info.size = selectQuery.value("size").toLongLong();
        m_caches.insert(selectQuery.value("id").toUInt(), info);
    }
}
