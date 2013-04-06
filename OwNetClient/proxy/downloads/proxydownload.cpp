#include "proxydownload.h"

#include "proxyrequest.h"
#include "proxyhandlersession.h"
#include "proxybytedownloadpart.h"
#include "proxystreamdownloadpart.h"
#include "proxylastdownloadpart.h"
#include "proxycachefiledownloadpart.h"
#include "proxyinputobject.h"

#include <QBuffer>
#include <QDebug>

/**
 * @brief Create ProxyDownload and ProxyInputObject based on request type.
 * Also registers the download in ProxyHandler to make sure it won't close as long as the download is active.
 * @param request ProxyRequest object with download info
 * @param handler ProxyHandler object
 * @param parent QObject parent
 */
ProxyDownload::ProxyDownload(ProxyRequest *request, ProxyHandlerSession *handlerSession, QObject *parent) :
    QObject(parent),
    m_proxyHandlerSession(handlerSession),
    m_nextReaderId(FirstReaderId),
    m_nextDownloadPartIndex(FirstDownloadPartIndex),
    m_inputObject(NULL)
{
    m_hashCode = request->hashCode();
}

ProxyDownload::~ProxyDownload()
{
    m_readersMutex.lock();
    m_readersMutex.unlock();
    m_downloadPartsMutex.lock();
    m_downloadPartsMutex.unlock();
}

/**
 * @brief Register a new download reader.
 * @return Download reader ID.
 */
int ProxyDownload::registerReader()
{
    int readerId = -1;

    QMutexLocker locker(&m_readersMutex);
    readerId = m_nextReaderId;
    m_nextReaderId++;
    m_readers.insert(readerId, FirstDownloadPartIndex);

    return readerId;
}

/**
 * @brief Deregister download reader with the given ID.
 * @param readerId ID of the reader to deregister
 */
void ProxyDownload::deregisterReader(int readerId)
{
    QMutexLocker locker(&m_readersMutex);
    m_readers.remove(readerId);
}

/**
 * @brief Count active download readers.
 * @return Number of active download readers
 */
int ProxyDownload::countRegisteredReaders()
{
    QMutexLocker mutexLocker(&m_readersMutex);

    return m_readers.count();
}

/**
 * @brief Downloads are shared if the input source is Web
 * @return True if the download should be shared
 */
bool ProxyDownload::shareDownload()
{
    if (inputObject())
        return inputObject()->inputType() == ProxyInputObject::Web;
    return false;
}

/**
 * @brief Get the next download part for the given reader. If found, increments the readers next part counter.
 * @param readerId ID of the reader
 * @return The next download part for the reader
 */
ProxyDownloadPart *ProxyDownload::downloadPart(int readerId)
{
    ProxyDownloadPart *downloadPart = NULL;
    if (!m_readers.contains(readerId))
        return NULL;

    QMutexLocker locker(&m_downloadPartsMutex);

    int i = m_readers.value(readerId);

    if (m_downloadParts.contains(i))
        downloadPart = m_downloadParts.value(i);

    if (downloadPart) {
        m_readers[readerId] = downloadPart->nextDownloadPartIndex();
        downloadPart->registerReader(readerId);
    }

    return downloadPart;
}

/**
 * @brief Replace the ProxyDownloadPart at the position and the next numParts with the given ProxyDownload
 * @param downloadPart The new ProxyDownloadPart
 * @param at Position of the ProxyDownloadPart
 * @param numParts Number of parts to replace
 */
void ProxyDownload::replaceDownloadParts(ProxyDownloadPart *downloadPart, int at)
{
    QMutexLocker locker(&m_downloadPartsMutex);

    ProxyDownloadPart *previousPart = m_downloadParts.value(at);

    m_downloadParts.insert(at, downloadPart);

    if (m_downloadParts.contains(downloadPart->nextDownloadPartIndex())) {
        ProxyDownloadPart *nextPart = m_downloadParts.value(downloadPart->nextDownloadPartIndex());
        nextPart->setParent(downloadPart);

        QList<int> previousPartsReaders;
        foreach (int key, m_readers.keys()) {
            int i = m_readers[key];
            if (i > at && i <= downloadPart->nextDownloadPartIndex())
                previousPartsReaders.append(key);
        }
        if (previousPartsReaders.count())
            nextPart->deletePreviousPartAfterReadersRegister(previousPart, previousPartsReaders);
        else
            delete previousPart;
    }
}

/**
 * @brief Read the response from the input object, creates a new download part.
 * Triggered when the response is ready.
 * @param ioDevice Stream from the input object to read.
 */
void ProxyDownload::readReply(QIODevice *ioDevice)
{
    {
        QMutexLocker locker(&m_downloadPartsMutex);

        int partIndex = m_nextDownloadPartIndex++;

        QObject *parent = this;
        if (partIndex > FirstDownloadPartIndex)
            parent = m_downloadParts.value(partIndex - 1);

        ProxyDownloadPart *part = NULL;
        if (shareDownload())
            part = new ProxyByteDownloadPart(new QByteArray(ioDevice->readAll()), m_nextDownloadPartIndex, parent);
        else
            part = new ProxyStreamDownloadPart(ioDevice, m_nextDownloadPartIndex, parent);
        m_downloadParts.insert(partIndex, part);
    }

    emit bytePartAvailable();
}

/**
 * @brief Triggered when the input object finished downloading the request.
 */
void ProxyDownload::inputObjectFinished()
{
    m_downloadPartsMutex.lock();
    m_downloadParts.insert(m_nextDownloadPartIndex++, new ProxyLastDownloadPart(this));
    m_downloadPartsMutex.unlock();

    emit downloadFinished();
}

/**
 * @brief Triggered when the input object found an error downloading the request and stops.
 */
void ProxyDownload::inputObjectError()
{
    m_downloadPartsMutex.lock();
    m_downloadParts.insert(m_nextDownloadPartIndex++, new ProxyLastDownloadPart(this, true));
    m_downloadPartsMutex.unlock();

    emit downloadFinished();
}
