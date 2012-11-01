#ifndef PROXYDOWNLOADS_H
#define PROXYDOWNLOADS_H

#include <QMap>
#include <QMutex>

class ProxyDownload;
class ProxyRequest;

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

    ProxyDownload *proxyDownload(ProxyRequest *request);

private:
    ProxyDownloads() {}

    QMap<int, ProxyDownload*> m_openDownloads;
    QMutex m_openDownloadsMutex;

    static ProxyDownloads *m_instance;
};

#endif // PROXYDOWNLOADS_H
