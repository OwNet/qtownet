#include "proxydownload.h"

#include "proxywebinputobject.h"
#include "proxystaticinputobject.h"
#include "proxycacheinputobject.h"
#include "proxyrequestbus.h"
#include "proxyrequest.h"
#include "proxyhandlersession.h"
#include "proxycacheoutputwriter.h"
#include "proxybytedownloadpart.h"
#include "proxystreamdownloadpart.h"
#include "proxylastdownloadpart.h"
#include "proxycachefiledownloadpart.h"

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
    m_inputObject(NULL),
    m_proxyHandlerSession(handlerSession),
    m_nextReaderId(FirstReaderId),
    m_shareDownload(false),
    m_nextDownloadPartIndex(FirstDownloadPartIndex)
{
    m_hashCode = request->hashCode();
    m_proxyHandlerDependentObjectId = m_proxyHandlerSession->registerDependentObject();

    if (request->isStaticResourceRequest()) {
        m_inputObject = new ProxyStaticInputObject(request, this);
    } else if (request->isLocalRequest()) {
        m_inputObject = new ProxyRequestBus(request, this);
    } else {
        m_inputObject = webInputObject(request);
    }

    if (m_shareDownload)
        new ProxyCacheOutputWriter(this, registerReader(), m_proxyHandlerSession);

    connect(m_inputObject, SIGNAL(finished()), this, SLOT(inputObjectFinished()));
    connect(m_inputObject, SIGNAL(failed()), this, SLOT(inputObjectError()));
    connect(m_inputObject, SIGNAL(readyRead(QIODevice*)), this, SLOT(readReply(QIODevice*)));
}

/**
 * @brief Register a new download reader.
 * @return Download reader ID.
 */
int ProxyDownload::registerReader()
{
    int readerId = -1;

    m_readersMutex.lock();
    readerId = m_nextReaderId;
    m_nextReaderId++;
    m_readers.insert(readerId, FirstDownloadPartIndex);
    m_readersMutex.unlock();

    return readerId;
}

/**
 * @brief Deregister download reader with the given ID.
 * @param readerId ID of the reader to deregister
 */
void ProxyDownload::deregisterReader(int readerId)
{
    m_readersMutex.lock();
    m_readers.remove(readerId);
    m_readersMutex.unlock();
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
 * @brief Start downloading the request. Triggered from outside.
 */
void ProxyDownload::startDownload()
{
    m_inputObject->startDownload();
}

/**
 * @brief Triggered after all readers finished reading the download. Deregisters from ProxyHandler.
 */
void ProxyDownload::close()
{
    m_proxyHandlerSession->deregisterDependentObject(m_proxyHandlerDependentObjectId);
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

    m_downloadPartsMutex.lock();
    int i = m_readers.value(readerId);

    if (m_downloadParts.contains(i))
        downloadPart = m_downloadParts.value(i);

    if (downloadPart) {
        m_readers[readerId] = downloadPart->nextDownloadPartIndex();
        downloadPart->registerReader(readerId);
    }

    m_downloadPartsMutex.unlock();

    return downloadPart;
}

/**
 * @brief Replace the ProxyDownloadPart at the position and the next numParts with the given ProxyDownload
 * @param downloadPart The new ProxyDownloadPart
 * @param at Position of the ProxyDownloadPart
 * @param numParts Number of parts to replace
 */
void ProxyDownload::replaceDownloadParts(ProxyCacheFileDownloadPart *downloadPart, int at)
{
    m_downloadPartsMutex.lock();

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

    m_downloadPartsMutex.unlock();
}

/**
 * @brief Read the response from the input object, creates a new download part.
 * Triggered when the response is ready.
 * @param ioDevice Stream from the input object to read.
 */
void ProxyDownload::readReply(QIODevice *ioDevice)
{
    m_downloadPartsMutex.lock();

    int partIndex = m_nextDownloadPartIndex++;

    QObject *parent = this;
    if (partIndex > FirstDownloadPartIndex)
        parent = m_downloadParts.value(partIndex - 1);

    ProxyDownloadPart *part = NULL;
    if (m_shareDownload)
        part = new ProxyByteDownloadPart(new QByteArray(ioDevice->readAll()), m_nextDownloadPartIndex, parent);
    else
        part = new ProxyStreamDownloadPart(ioDevice, m_nextDownloadPartIndex, parent);
    m_downloadParts.insert(partIndex, part);

    m_downloadPartsMutex.unlock();

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

/**
 * @brief Create an input object for web request. Download from cache if available.
 * @param request Web request to download.
 * @return Web input object.
 */
ProxyInputObject *ProxyDownload::webInputObject(ProxyRequest *request)
{
    if (request->requestType() == ProxyRequest::GET) {
        ProxyCacheInputObject *cacheInputObject = new ProxyCacheInputObject(request, this);
        if (cacheInputObject->exists())
            return cacheInputObject;
    }

    m_shareDownload = true;
    return new ProxyWebInputObject(request, this);
}
