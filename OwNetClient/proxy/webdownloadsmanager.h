#ifndef WEBDOWNLOADSMANAGER_H
#define WEBDOWNLOADSMANAGER_H

#include <QMutex>
#include <QMap>
#include <QNetworkProxy>

#include "idatabaseupdatelistener.h"

class GDSFClock;
class ProxyTrafficCounter;
class CacheExceptions;
class WebDownload;
class CacheLocations;
class ProxyRequest;
class WebReader;
class ProxyHandlerSession;
class CacheAccessManager;

class WebDownloadsManager : public IDatabaseUpdateListener
{
public:
    static WebDownloadsManager *instance() {
        static QMutex mutex;
        if (!m_instance) {
            mutex.lock();
            if (!m_instance)
                m_instance = new WebDownloadsManager;
            mutex.unlock();
        }

        return m_instance;
    }
    ~WebDownloadsManager();

    QIODevice *getStream(ProxyRequest *request, WebReader *reader, ProxyHandlerSession *handlerSession, bool *finished);
    void deregisterDownloadReader(WebDownload *proxyDownload, int readerId);

    void logCacheAccess(uint cacheId, qint64 size = -1, int numAccesses = 1);
    void saveCacheAccesses();
    GDSFClock *gdsfClock() const { return m_gdsfClock; }
    ProxyTrafficCounter *trafficCounter() const { return m_trafficCounter; }

    QNetworkProxy applicationProxy() const;
    bool isSetApplicationProxy() const;
    void setApplicationProxy(const QString &ip, int port);

    /// IDatabaseUpdateListener functions
    QString tableName() const { return "client_caches"; }
    void tableUpdated(IDatabaseUpdateQuery *query);

    bool containsCacheLocation(uint cacheId, const QString &clientId) const;
    bool isCacheAvailable(uint cacheId) const;

    bool containsCacheExceptionFor(const QString &url);

private:
    WebDownloadsManager();
    void initCacheLocations();
    void addCacheLocation(uint cacheId, const QString &clientId, const QString &dateCreated);
    bool shouldRefresh(ProxyRequest *request);

    QMutex m_openDownloadsMutex;
    QMap<uint, WebDownload*> m_downloads;

    GDSFClock *m_gdsfClock;
    ProxyTrafficCounter *m_trafficCounter;
    CacheExceptions *m_cacheExceptions;
    CacheAccessManager *m_cacheAccessManager;

    int m_proxyPort;
    QString m_proxyIp;

    static WebDownloadsManager *m_instance;
};

#endif // WEBDOWNLOADSMANAGER_H
