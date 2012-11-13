#include "proxydownloads.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "gdsfclock.h"
#include "proxyhandler.h"

ProxyDownloads *ProxyDownloads::m_instance = 0;

/**
 * @brief Creates a new ProxyDownload, or returns an active one if available.
 * @param request Request to retrieve a download by
 * @param handler ProxyHandler parent class
 * @param downloadReaderId Reference to reader ID varibale to set
 * @return ProxyDownload object
 */
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

/**
 * @brief Deregisters reader from an active download. Disposes the download if no readers left.
 * @param proxyDownload Download to deregister from
 * @param readerId ID of the reader
 */
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

    if (close) {
        proxyDownload->close();
        delete proxyDownload;
    }
}

ProxyDownloads::ProxyDownloads()
{
    m_gdsfClock = new GDSFClock();
}
