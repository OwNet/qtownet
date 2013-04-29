#ifndef PROXYDOWNLOADS_H
#define PROXYDOWNLOADS_H

#include <QMap>
#include <QMutex>
#include <QNetworkProxy>

#include "idatabaseupdatelistener.h"

class ProxyDownload;
class ProxyInputObject;
class ProxyRequest;
class ProxyHandlerSession;
class GDSFClock;
class ProxyTrafficCounter;
class ProxyOutputWriter;
class ProxyCacheLocations;
class CacheExceptions;

/**
 * @brief Contains a list of all active downloads. Creates a new download or reuses an active one on request.
 * Singleton.
 */
class ProxyDownloads : public IDatabaseUpdateListener
{
public:
    static ProxyDownloads *instance() {
        static QMutex mutex;
        if (!m_instance) {
            mutex.lock();
            if (!m_instance)
                m_instance = new ProxyDownloads;
            mutex.unlock();
        }

        return m_instance;
    }
    ~ProxyDownloads();

    ProxyDownload *proxyDownload(ProxyRequest *request, ProxyHandlerSession *handlerSession, ProxyOutputWriter *outputWriter, int &downloadReaderId);
    void deregisterDownloadReader(ProxyDownload *proxyDownload, int readerId);

    GDSFClock *gdsfClock() const { return m_gdsfClock; }
    ProxyTrafficCounter *trafficCounter() const { return m_trafficCounter; }

    QNetworkProxy applicationProxy() const;
    bool isSetApplicationProxy() const;
    void setApplicationProxy(const QString &ip, int port);

    /// IDatabaseUpdateListener functions
    QString tableName() const { return "client_caches"; }
    void tableUpdated(IDatabaseUpdateQuery *query);

    bool containsCacheLocation(uint cacheId, const QString &clientId) const;

private:
    ProxyDownloads();
    ProxyInputObject *newInputObject(ProxyRequest *request, ProxyHandlerSession *handlerSession);
    void connectDownloadAndOutputWriter(ProxyDownload *download, ProxyOutputWriter *outputWriter) const;
    void initCacheLocations();
    void addCacheLocation(uint cacheId, const QString &clientId, const QString &dateCreated);
    bool shouldRefresh(ProxyRequest *request);

    QMap<uint, ProxyDownload*> m_openDownloads;
    QMutex m_openDownloadsMutex;
    QMap<uint, ProxyCacheLocations*> m_cacheLocations;

    GDSFClock *m_gdsfClock;
    ProxyTrafficCounter *m_trafficCounter;
    CacheExceptions *m_cacheExceptions;

    int m_proxyPort;
    QString m_proxyIp;

    static ProxyDownloads *m_instance;
};

#endif // PROXYDOWNLOADS_H
