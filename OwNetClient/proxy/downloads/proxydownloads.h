#ifndef PROXYDOWNLOADS_H
#define PROXYDOWNLOADS_H

#include <QMap>
#include <QMutex>

class ProxyDownload;
class ProxyRequest;
class ProxyHandler;
class GDSFClock;

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

    ProxyDownload *proxyDownload(ProxyRequest *request, ProxyHandler *handler, int &downloadReaderId);
    void deregisterDownloadReader(ProxyDownload *proxyDownload, int readerId);
    GDSFClock *gdsfClock() { return m_gdsfClock; }

private:
    ProxyDownloads();

    QMap<int, ProxyDownload*> m_openDownloads;
    QMutex m_openDownloadsMutex;
    GDSFClock *m_gdsfClock;

    static ProxyDownloads *m_instance;
};

#endif // PROXYDOWNLOADS_H
