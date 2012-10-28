#include "proxyoutputwriter.h"

#include "proxydownload.h"
#include "proxydownloads.h"
#include "proxyrequest.h"

ProxyOutputWriter::ProxyOutputWriter(QObject *parent) :
    QObject(parent), m_proxyDownload(NULL), m_downloadReaderId(-1), m_lastPartRead(0)
{
    m_proxyDownloads = ProxyDownloads::instance();
}

void ProxyOutputWriter::createDownload(ProxyRequest *request)
{
    m_proxyDownload = m_proxyDownloads->proxyDownload(request);

    registerDownload();

    m_proxyDownload->startDownload();
}

void ProxyOutputWriter::registerDownload(ProxyDownload *download)
{
    m_proxyDownload = download;
    registerDownload();
}

void ProxyOutputWriter::close()
{
    m_proxyDownload->deregisterReader(m_downloadReaderId);

    disconnect(m_proxyDownload, SIGNAL(downloadFinished()), this, SLOT(downloadFinished()));
    disconnect(m_proxyDownload, SIGNAL(bytePartAvailable()), this, SLOT(readAvailableParts()));
}

void ProxyOutputWriter::registerDownload()
{
    connect(m_proxyDownload, SIGNAL(downloadFinished()), this, SLOT(downloadFinished()), Qt::QueuedConnection);
    connect(m_proxyDownload, SIGNAL(bytePartAvailable()), this, SLOT(readAvailableParts()), Qt::QueuedConnection);

    m_downloadReaderId = m_proxyDownload->registerReader();
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
