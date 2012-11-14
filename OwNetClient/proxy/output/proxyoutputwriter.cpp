#include "proxyoutputwriter.h"

#include "proxydownload.h"
#include "proxydownloads.h"
#include "proxyrequest.h"
#include "proxyhandler.h"
#include "proxydownloadpart.h"

ProxyOutputWriter::ProxyOutputWriter(ProxyHandler *proxyHandler, QObject *parent) :
    QObject(parent), m_proxyHandler(proxyHandler), m_proxyDownload(NULL), m_downloadReaderId(-1), m_lastPartRead(0), m_closed(false)
{
    m_proxyDownloads = ProxyDownloads::instance();
}

/**
 * @brief Force finish downloading request, triggered from outside.
 */
void ProxyOutputWriter::finish()
{
    m_readingMutex.lock();
    close();
    m_readingMutex.unlock();
}

/**
 * @brief Create ProxyDownload object and connect its signals.
 * @param request Request to download
 */
void ProxyOutputWriter::createDownload(ProxyRequest *request)
{
    m_proxyDownload = m_proxyDownloads->proxyDownload(request, m_proxyHandler, m_downloadReaderId);
    connectProxyDownload();

    m_proxyDownload->startDownload();
}

/**
 * @brief Deregister as a ProxyDownload reader and disconnect from all ProxyDownload signals.
 */
void ProxyOutputWriter::close()
{
    virtualClose();

    if (m_proxyDownload) {
        m_proxyDownloads->deregisterDownloadReader(m_proxyDownload, m_downloadReaderId);

        disconnect(m_proxyDownload);
    }

}

/**
 * @brief Read all byte parts available in the ProxyDownload.
 * Checks if download finished and closes the output writer if it did.
 */
void ProxyOutputWriter::readAvailableParts()
{
    m_readingMutex.lock();
    if (m_closed)
        return;
    bool downloadFinished = false;
    bool callClose = false;

    ProxyDownloadPart *downloadPart = NULL;
    do {
        downloadPart = m_proxyDownload->downloadPart(m_downloadReaderId);
        if (downloadPart) {
            if (downloadPart->isError())
                error();

            if (downloadPart->isLast()) {
                downloadFinished = true;
                break;
            } else {
                read(downloadPart->stream());
            }
        }
    } while (downloadPart);

    if (downloadFinished) {
        m_closed = true;
        callClose = true;
    }

    m_readingMutex.unlock();

    if (callClose) {
        close();
        emit finished();
    } else {
        emit iAmActive();
    }
}

/**
 * @brief Connects slots to signals in the ProxyDownload.
 */
void ProxyOutputWriter::connectProxyDownload()
{
    connect(m_proxyDownload, SIGNAL(downloadFinished()), this, SLOT(readAvailableParts()), Qt::QueuedConnection);
    connect(m_proxyDownload, SIGNAL(bytePartAvailable()), this, SLOT(readAvailableParts()), Qt::QueuedConnection);
}
