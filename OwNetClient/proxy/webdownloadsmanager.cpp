#include "webdownloadsmanager.h"

#include "proxywebdownload.h"
#include "proxyrequest.h"
#include "gdsfclock.h"
#include "proxyhandlersession.h"
#include "proxytrafficcounter.h"
#include "proxywebinputobject.h"
#include "proxycacheinputobject.h"
#include "proxycacheoutputwriter.h"
#include "databaseselectquery.h"
#include "cachelocations.h"
#include "session.h"
#include "databaseupdatequery.h"
#include "cacheexceptions.h"
#include "cachehelper.h"
#include "databasesettings.h"

WebDownloadsManager *WebDownloadsManager::m_instance = 0;

WebDownloadsManager::~WebDownloadsManager()
{
    delete m_trafficCounter;
    delete m_gdsfClock;
}

/**
 * @brief Creates a new ProxyWebDownload, or returns an active one if available.
 * @param request Request to retrieve a download by
 * @param handler ProxyHandler parent class
 * @param downloadReaderId Reference to reader ID varibale to set
 * @return ProxyWebDownload object
 */
QIODevice *WebDownloadsManager::getStream(ProxyRequest *request, ProxyWebReader *reader, ProxyHandlerSession *handlerSession, bool *finished)
{
    m_openDownloadsMutex.lock();
    // ADD check before: !m_cacheExceptions->containsExceptionFor(request->url())
    uint cacheId = request->hashCode();

    CacheLocations *locations = NULL;
    if (m_cacheLocations.contains(cacheId)) {
        locations = m_cacheLocations.value(cacheId);
    } else {
        locations = new CacheLocations;
        m_cacheLocations.insert(cacheId, locations);
    }
    QIODevice *stream = locations->getStream(request, reader, handlerSession, finished, shouldRefresh(request));
    m_openDownloadsMutex.unlock();
    return stream;
}

QNetworkProxy WebDownloadsManager::applicationProxy() const
{
    if (isSetApplicationProxy())
        return QNetworkProxy(QNetworkProxy::HttpProxy, m_proxyIp, m_proxyPort);
    return QNetworkProxy();
}

bool WebDownloadsManager::isSetApplicationProxy() const
{
    return m_proxyPort != -1;
}

void WebDownloadsManager::setApplicationProxy(const QString &ip, int port)
{
    m_proxyIp = ip;
    m_proxyPort = port;
}

/**
 * @brief IDatabaseUpdateListener event triggered when a query changes the table client_caches.
 * @param query
 */
void WebDownloadsManager::tableUpdated(IDatabaseUpdateQuery *query)
{
    QString myClientId = DatabaseSettings().clientId();
    QVariantList columns = query->columnsForListeners();
    for (int i = 0; i < columns.count(); ++i) {
        QVariantMap columnsMap = columns.at(i).toMap();

        if (query->type() == IDatabaseUpdateQuery::InsertOrUpdate) {
            if (myClientId == columnsMap.value("client_id").toString())
                continue;

            addCacheLocation(columnsMap.value("cache_id").toUInt(),
                             columnsMap.value("client_id").toString(),
                             columnsMap.value("date_created").toString());
        } else if (query->type() == IDatabaseUpdateQuery::Delete) {
            if (m_cacheLocations.contains(columnsMap.value("cache_id").toUInt())) {
                m_cacheLocations.value(columnsMap.value("cache_id").toUInt())
                        ->removeLocation(columnsMap.value("client_id").toString());
            }
        }
    }
}

bool WebDownloadsManager::containsCacheLocation(uint cacheId, const QString &clientId) const
{
    if (!m_cacheLocations.contains(cacheId))
        return false;

    return m_cacheLocations.value(cacheId)->containsLocation(clientId);
}

bool WebDownloadsManager::isCacheAvailable(uint cacheId) const
{
    if (m_cacheLocations.contains(cacheId)) {
        QVariantMap availableClients = Session().availableClients();
        QList<ProxyWebDownload*> locations = m_cacheLocations.value(cacheId)->sortedLocations();

        foreach (ProxyWebDownload *location, locations) {
            if (location->isLocalCache())
                return true;

            if (location->isWeb())
                continue;

            if (availableClients.contains(location->clientId()))
                return true;
        }
    }
    return false;
}

WebDownloadsManager::WebDownloadsManager()
    : m_proxyPort(-1)
{
    m_gdsfClock = new GDSFClock;
    m_trafficCounter = new ProxyTrafficCounter;
    m_cacheExceptions = new CacheExceptions;

    initCacheLocations();

    DatabaseUpdateQuery::registerListener(this);
}

void WebDownloadsManager::initCacheLocations()
{
    DatabaseSelectQuery query("client_caches");
    while (query.next()) {
        addCacheLocation(query.value("cache_id").toUInt(),
                         query.value("client_id").toString(),
                         query.value("date_created").toString());
    }
}

void WebDownloadsManager::addCacheLocation(uint cacheId, const QString &clientId, const QString &dateCreated)
{
    addCacheLocation(cacheId, clientId, dateCreated, new ProxyWebDownload(cacheId, clientId));
}

void WebDownloadsManager::addCacheLocation(uint cacheId, const QString &clientId, const QString &dateCreated, ProxyWebDownload *download)
{
    if (m_cacheLocations.contains(cacheId)) {
        m_cacheLocations.value(cacheId)->addLocation(clientId, dateCreated, download);
    } else {
        CacheLocations *locations = new CacheLocations;
        locations->addLocation(clientId, dateCreated, download);
        m_cacheLocations.insert(cacheId, locations);
    }
}

bool WebDownloadsManager::shouldRefresh(ProxyRequest *request)
{
    if (request->domain() == "local")
        return false;

    return Session().isRefreshSession() || request->isRefreshRequest();
}
