#ifndef WEBDOWNLOADSMANAGER_H
#define WEBDOWNLOADSMANAGER_H

#include <QMutex>
#include <QMap>
#include <QNetworkProxy>

#include "idatabaseupdatelistener.h"

class GDSFClock;
class ProxyTrafficCounter;
class CacheExceptions;
class ProxyWebDownload;
class CacheLocations;
class ProxyRequest;
class ProxyWebReader;
class ProxyHandlerSession;

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

    QIODevice *getStream(ProxyRequest *request, ProxyWebReader *reader, ProxyHandlerSession *handlerSession, bool *finished);
    void deregisterDownloadReader(ProxyWebDownload *proxyDownload, int readerId);

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

private:
    WebDownloadsManager();
    void initCacheLocations();
    void addCacheLocation(uint cacheId, const QString &clientId, const QString &dateCreated);
    void addCacheLocation(uint cacheId, const QString &clientId, const QString &dateCreated, ProxyWebDownload *download);
    bool shouldRefresh(ProxyRequest *request);

    QMutex m_openDownloadsMutex;
    QMap<uint, CacheLocations*> m_cacheLocations;

    GDSFClock *m_gdsfClock;
    ProxyTrafficCounter *m_trafficCounter;
    CacheExceptions *m_cacheExceptions;

    int m_proxyPort;
    QString m_proxyIp;

    static WebDownloadsManager *m_instance;
};

#endif // WEBDOWNLOADSMANAGER_H
