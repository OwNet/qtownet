#include "proxydownloads.h"

#include "proxydownload.h"
#include "proxyrequest.h"

ProxyDownloads *ProxyDownloads::m_instance = 0;

ProxyDownload *ProxyDownloads::proxyDownload(ProxyRequest *request)
{
    ProxyDownload *download = NULL;

    m_openDownloadsMutex.lock();

    if (m_openDownloads.contains(request->hashCode())) {
        download = m_openDownloads.value(request->hashCode());
    } else {
        download = new ProxyDownload(request);
        if (download->shareDownload())
            m_openDownloads.insert(request->hashCode(), download);
    }

    m_openDownloadsMutex.unlock();

    return download;
}
