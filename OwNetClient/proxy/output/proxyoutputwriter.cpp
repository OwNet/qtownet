#include "proxyoutputwriter.h"

#include "proxydownload.h"
#include "proxydownloads.h"
#include "proxyrequest.h"
#include "proxyhandler.h"

ProxyOutputWriter::ProxyOutputWriter(ProxyHandler *proxyHandler) :
    QObject(proxyHandler), m_proxyHandler(proxyHandler), m_proxyDownload(NULL), m_downloadReaderId(-1), m_lastPartRead(0)
{
    m_proxyDownloads = ProxyDownloads::instance();
}

void ProxyOutputWriter::createDownload(ProxyRequest *request)
{
    m_proxyDownload = m_proxyDownloads->proxyDownload(request, m_proxyHandler, m_downloadReaderId);
    //m_proxyDownload = new ProxyDownload(request, m_proxyHandler, m_proxyHandler);
    //m_downloadReaderId = m_proxyDownload->registerReader();

    connect(m_proxyDownload, SIGNAL(downloadFinished()), this, SLOT(downloadFinished()), Qt::QueuedConnection);
    connect(m_proxyDownload, SIGNAL(bytePartAvailable()), this, SLOT(readAvailableParts()), Qt::QueuedConnection);

    m_proxyDownload->startDownload();
}

void ProxyOutputWriter::close()
{
    if (m_proxyDownload) {
        m_proxyDownloads->deregisterDownloadReader(m_proxyDownload, m_downloadReaderId);
        //m_proxyDownload->deregisterReader(m_downloadReaderId);

        disconnect(m_proxyDownload);
    }
}

void ProxyOutputWriter::readAvailableParts()
{
    m_readingMutex.lock();
    QIODevice *ioDevice = NULL;
    do {
        ioDevice = m_proxyDownload->bytePart(m_downloadReaderId);
        if (ioDevice) {
            read(ioDevice);
        }
    } while (ioDevice);
    m_readingMutex.unlock();

    emit iAmActive();
}

void ProxyOutputWriter::downloadFinished()
{
    readAvailableParts();
    close();
}
