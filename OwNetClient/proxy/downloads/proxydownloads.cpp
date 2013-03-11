#include "proxydownloads.h"

#include "proxydownload.h"
#include "proxyrequest.h"
#include "gdsfclock.h"
#include "proxyhandlersession.h"
#include "proxytrafficcounter.h"
#include "proxywebinputobject.h"
#include "proxystaticinputobject.h"
#include "proxycacheinputobject.h"
#include "proxycacheoutputwriter.h"
#include "proxyrequestbus.h"
#include "databaseselectquery.h"
#include "proxycachelocations.h"
#include "session.h"
#include "databaseupdatequery.h"

ProxyDownloads *ProxyDownloads::m_instance = 0;

ProxyDownloads::~ProxyDownloads()
{
    delete m_trafficCounter;
    delete m_gdsfClock;
}

/**
 * @brief Creates a new ProxyDownload, or returns an active one if available.
 * @param request Request to retrieve a download by
 * @param handler ProxyHandler parent class
 * @param downloadReaderId Reference to reader ID varibale to set
 * @return ProxyDownload object
 */
ProxyDownload *ProxyDownloads::proxyDownload(ProxyRequest *request, ProxyHandlerSession *handlerSession, ProxyOutputWriter *outputWriter, int &downloadReaderId)
{
    ProxyDownload *download = NULL;
    ProxyInputObject *inputObject = NULL;

    m_openDownloadsMutex.lock();

    if (m_openDownloads.contains(request->hashCode())) {
        /// If such download already exists, use it
        download = m_openDownloads.value(request->hashCode());
    } else {
        /// Create new InputObject and ProxyDownload instances
        inputObject = newInputObject(request, handlerSession);
        download = new ProxyDownload(request, handlerSession, handlerSession);
        download->setInputObject(inputObject);

        /// Connect signals in the InputObject to slots in the ProxyDownload
        QObject::connect(inputObject, SIGNAL(finished()), download, SLOT(inputObjectFinished()));
        QObject::connect(inputObject, SIGNAL(failed()), download, SLOT(inputObjectError()));
        QObject::connect(inputObject, SIGNAL(readyRead(QIODevice*)), download, SLOT(readReply(QIODevice*)));

        /// If the source is Web, save the download in the downloads map
        if (inputObject->inputType() == ProxyInputObject::Web)
            m_openDownloads.insert(request->hashCode(), download);
    }
    downloadReaderId = download->registerReader();

    m_openDownloadsMutex.unlock();

    connectDownloadAndOutputWriter(download, outputWriter);

    if (inputObject) {
        /// Create a cache output writer if the input source is Web
        if (inputObject->inputType() == ProxyInputObject::Web)
            connectDownloadAndOutputWriter(download,
                                           new ProxyCacheOutputWriter(download, download->registerReader(), handlerSession));

        /// Starts the download
        inputObject->startDownload();
    }

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

    if (close)
        proxyDownload->close();
}

QNetworkProxy ProxyDownloads::applicationProxy() const
{
    if (isSetApplicationProxy())
        return QNetworkProxy(QNetworkProxy::HttpProxy, m_proxyIp, m_proxyPort);
    return QNetworkProxy();
}

bool ProxyDownloads::isSetApplicationProxy() const
{
    return m_proxyPort != -1;
}

void ProxyDownloads::setApplicationProxy(const QString &ip, int port)
{
    m_proxyIp = ip;
    m_proxyPort = port;
}

/**
 * @brief IDatabaseUpdateListener event triggered when a query changes the table client_caches.
 * @param query
 */
void ProxyDownloads::tableUpdated(IDatabaseUpdateQuery *query)
{
    QVariantMap columns = query->columns();
    uint id = columns.value("cache_id").toUInt();
    QString clientId = columns.value("client_id").toString();

    if (query->type() == IDatabaseUpdateQuery::InsertOrUpdate) {
        addCacheLocation(id, clientId, columns.value("date_created").toString());
    } else if (query->type() == IDatabaseUpdateQuery::Delete) {
        if (m_cacheLocations.contains(id)) {
            m_cacheLocations.value(id)->removeLocation(clientId);
        }
    }
}

ProxyDownloads::ProxyDownloads()
    : m_proxyPort(-1)
{
    m_gdsfClock = new GDSFClock;
    m_trafficCounter = new ProxyTrafficCounter;

    initCacheLocations();

    DatabaseUpdateQuery::registerListener(this);
}

/**
 * @brief Create a new input object based on the request.
 * @param request
 * @param handlerSession
 * @return New ProxyInputObject object
 */
ProxyInputObject *ProxyDownloads::newInputObject(ProxyRequest *request, ProxyHandlerSession *handlerSession)
{
    ProxyInputObject *inputObject = NULL;

    if (request->isStaticResourceRequest()) {
        inputObject = new ProxyStaticInputObject(request, handlerSession);
    } else if (request->isLocalRequest()) {
        inputObject = new ProxyRequestBus(request, handlerSession);
    } else {
        QVariantMap availableClients = Session().availableClients();

        if (m_cacheLocations.contains(request->hashCode())) {
            QList<ProxyCacheLocation*> locations = m_cacheLocations.value(request->hashCode())->sortedLocations();
            foreach (ProxyCacheLocation *location, locations) {
                if (location->isLocal()) {
                    ProxyCacheInputObject *cacheInput = new ProxyCacheInputObject(request, handlerSession);
                    if (cacheInput->exists()) {
                        inputObject = cacheInput;
                        break;
                    } else {
                        continue;
                    }
                } else {
                    if (!location->isWeb() && !availableClients.contains(location->clientId()))
                        continue;

                    ProxyWebInputObject *webObject = new ProxyWebInputObject(request, handlerSession);
                    if (!location->isWeb())
                        webObject->setProxy(availableClients.value(location->clientId()).toString());
                    break;
                }
            }
        }
        if (!inputObject) {
            inputObject = new ProxyWebInputObject(request, handlerSession);
        }
    }

    return inputObject;
}

/**
 * @brief Connect signals in the ProxyDownload to slots in the OutputWriter
 * @param download
 * @param outputWriter
 */
void ProxyDownloads::connectDownloadAndOutputWriter(ProxyDownload *download, ProxyOutputWriter *outputWriter) const
{
    QObject::connect(download, SIGNAL(downloadFinished()), outputWriter, SLOT(readAvailableParts()), Qt::QueuedConnection);
    QObject::connect(download, SIGNAL(bytePartAvailable()), outputWriter, SLOT(readAvailableParts()), Qt::QueuedConnection);
}

void ProxyDownloads::initCacheLocations()
{
    DatabaseSelectQuery query("client_caches");
    while (query.next()) {
        addCacheLocation(query.value("cache_id").toUInt(),
                         query.value("client_id").toString(),
                         query.value("date_created").toString());
    }
}

void ProxyDownloads::addCacheLocation(uint cacheId, const QString &clientId, const QString &dateCreated)
{
    if (m_cacheLocations.contains(cacheId)) {
        m_cacheLocations.value(cacheId)->addLocation(clientId, dateCreated);
    } else {
        ProxyCacheLocations *locations = new ProxyCacheLocations;
        locations->addLocation(clientId, dateCreated);
        m_cacheLocations.insert(cacheId, locations);
    }
}
