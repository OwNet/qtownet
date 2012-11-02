#include "proxydownloads.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "proxyhandler.h"

ProxyDownloads *ProxyDownloads::m_instance = 0;

ProxyDownload *ProxyDownloads::proxyDownload(ProxyRequest *request, ProxyHandler *handler, int &downloadReaderId)
{
    ProxyDownload *download = NULL;

    m_openDownloadsMutex.lock();

    if (m_openDownloads.contains(request->hashCode())) {
        download = m_openDownloads.value(request->hashCode());
    } else {
        download = new ProxyDownload(request, handler, handler);
        if (download->shareDownload())
            m_openDownloads.insert(request->hashCode(), download);
    }
    downloadReaderId = download->registerReader();

    m_openDownloadsMutex.unlock();

    return download;
}

void ProxyDownloads::deregisterDownloadReader(ProxyDownload *proxyDownload, int readerId)
{
    proxyDownload->deregisterReader(readerId);
    bool close = false;

    m_openDownloadsMutex.lock();
    if (!proxyDownload->countRegisteredReaders()) {
        m_openDownloads.remove(proxyDownload->hashCode());
        close = true;
    }
    m_openDownloadsMutex.unlock();

    if (close)
        proxyDownload->close();
}
