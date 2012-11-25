#include "proxyoutputwriter.h"

#include "proxydownload.h"
#include "proxydownloads.h"
#include "proxyrequest.h"
#include "proxyhandlersession.h"
#include "proxydownloadpart.h"

ProxyOutputWriter::ProxyOutputWriter(ProxyHandlerSession *proxyHandlerSession)
    : QObject(proxyHandlerSession), m_proxyHandlerSession(proxyHandlerSession), m_proxyDownload(NULL), m_downloadReaderId(-1), m_lastPartRead(0), m_closed(false)
{
    m_proxyHandlerDependentObjectId = m_proxyHandlerSession->registerDependentObject();
    m_proxyDownloads = ProxyDownloads::instance();

    connect(m_proxyHandlerSession, SIGNAL(shouldForceQuit()), this, SLOT(forceQuit()));
}

/**
 * @brief Force finish downloading request, triggered from outside.
 */
void ProxyOutputWriter::forceQuit()
{
    m_readingMutex.lock();
    if (m_closed) {
        m_readingMutex.unlock();
        return;
    }
    m_closed = true;
    m_readingMutex.unlock();

    close();
}

/**
 * @brief Create ProxyDownload object and connect its signals.
 * @param request Request to download
 */
void ProxyOutputWriter::createDownload(ProxyRequest *request)
{
    m_proxyDownload = m_proxyDownloads->proxyDownload(request, m_proxyHandlerSession, m_downloadReaderId);
    connectProxyDownload();

    m_proxyDownload->startDownload();
}

/**
 * @brief Deregister as a ProxyDownload reader and disconnect from all ProxyDownload signals.
 */
void ProxyOutputWriter::close()
{
    if (m_proxyDownload) {
        disconnect(m_proxyDownload);

        m_proxyDownloads->deregisterDownloadReader(m_proxyDownload, m_downloadReaderId);
        m_proxyDownload = NULL;
    }

    virtualClose();
    m_proxyHandlerSession->deregisterDependentObject(m_proxyHandlerDependentObjectId);
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