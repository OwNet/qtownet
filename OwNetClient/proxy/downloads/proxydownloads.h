#ifndef PROXYDOWNLOADS_H
#define PROXYDOWNLOADS_H

#include <QMap>
#include <QMutex>

class ProxyDownload;
class ProxyRequest;
class ProxyHandlerSession;
class GDSFClock;
class ProxyTrafficCounter;

/**
 * @brief Contains a list of all active downloads. Creates a new download or reuses an active one on request.
 * Singleton.
 */
class ProxyDownloads
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

    ProxyDownload *proxyDownload(ProxyRequest *request, ProxyHandlerSession *handlerSession, int &downloadReaderId);
    void deregisterDownloadReader(ProxyDownload *proxyDownload, int readerId);
    GDSFClock *gdsfClock() const { return m_gdsfClock; }
    ProxyTrafficCounter *trafficCounter() const { return m_trafficCounter; }

private:
    ProxyDownloads();

    QMap<int, ProxyDownload*> m_openDownloads;
    QMutex m_openDownloadsMutex;
    GDSFClock *m_gdsfClock;
    ProxyTrafficCounter *m_trafficCounter;

    static ProxyDownloads *m_instance;
};

#endif // PROXYDOWNLOADS_H
