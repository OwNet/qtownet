#ifndef CACHEACCESSMANAGER_H
#define CACHEACCESSMANAGER_H

#include <QObject>

#include <QMap>
#include <QSet>
#include <QMutex>

struct CacheInfo {
    int numAccesses;
    qint64 size;
    double accessValue;
};

class CacheAccessManager : public QObject
{
    Q_OBJECT
public:
    explicit CacheAccessManager(QObject *parent = 0);

    void logCacheAccess(uint cacheId, qint64 size = -1, int numAccesses = 1);
    void saveToDatabase();

private:
    void init();

    QSet<uint> m_cachesToUpdate;
    QSet<uint> m_cachesToInsert;
    QMap<uint, CacheInfo> m_caches;
    QMutex m_mutex;
};

#endif // CACHEACCESSMANAGER_H
