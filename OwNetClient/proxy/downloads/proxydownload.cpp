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
    QObject(parent), m_inputObject(NULL), m_proxyHandlerSession(handlerSession), m_nextReaderId(1), m_shareDownload(false)
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
    m_readers.insert(readerId, 0);
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

    if (m_downloadParts.count() > i)
        downloadPart = m_downloadParts.at(i);

    if (downloadPart)
        m_readers[readerId] = i + 1;
    m_downloadPartsMutex.unlock();

    return downloadPart;
}

/**
 * @brief Read the response from the input object, creates a new download part. Triggered when the response is ready.
 * @param ioDevice Stream from the input object to read.
 */
void ProxyDownload::readReply(QIODevice *ioDevice)
{
    m_downloadPartsMutex.lock();
    if (m_shareDownload)
        m_downloadParts.append(new ProxyByteDownloadPart(new QByteArray(ioDevice->readAll()), this));
    else
        m_downloadParts.append(new ProxyStreamDownloadPart(ioDevice, this));
    m_downloadPartsMutex.unlock();

    emit bytePartAvailable();
}

/**
 * @brief Triggered when the input object finished downloading the request.
 */
void ProxyDownload::inputObjectFinished()
{
    m_downloadPartsMutex.lock();
    m_downloadParts.append(new ProxyLastDownloadPart(this));
    m_downloadPartsMutex.unlock();

    emit downloadFinished();
}

/**
 * @brief Triggered when the input object found an error downloading the request and stops.
 */
void ProxyDownload::inputObjectError()
{
    m_downloadPartsMutex.lock();
    m_downloadParts.append(new ProxyLastDownloadPart(this, true));
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
