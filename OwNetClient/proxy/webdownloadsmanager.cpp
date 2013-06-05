#include "webdownloadsmanager.h"

#include "webdownload.h"
#include "proxyrequest.h"
#include "gdsfclock.h"
#include "proxyhandlersession.h"
#include "proxytrafficcounter.h"
#include "databaseselectquery.h"
#include "cachelocations.h"
#include "session.h"
#include "databaseupdatequery.h"
#include "cacheexceptions.h"
#include "cachehelper.h"
#include "databasesettings.h"
#include "cacheaccessmanager.h"

#include <QDateTime>

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
QIODevice *WebDownloadsManager::getStream(ProxyRequest *request, WebReader *reader, ProxyHandlerSession *handlerSession, bool *finished)
{
    m_openDownloadsMutex.lock();
    // ADD check before: !m_cacheExceptions->containsExceptionFor(request->url())
    uint cacheId = request->hashCode();

    WebDownload *download = NULL;
    if (m_downloads.contains(cacheId)) {
        download = m_downloads.value(cacheId);
    } else {
        download = new WebDownload(cacheId);
        m_downloads.insert(cacheId, download);
    }
    m_openDownloadsMutex.unlock();
    return download->getStream(request, reader, handlerSession, shouldRefresh(request), finished);
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
    QVariantList columns = query->columnsForListeners();
    for (int i = 0; i < columns.count(); ++i) {
        QVariantMap columnsMap = columns.at(i).toMap();

        if (query->type() == IDatabaseUpdateQuery::InsertOrUpdate) {
            addCacheLocation(columnsMap.value("cache_id").toUInt(),
                             columnsMap.value("client_id").toString(),
                             columnsMap.value("date_created").toString());
        } else if (query->type() == IDatabaseUpdateQuery::Delete) {
            if (m_downloads.contains(columnsMap.value("cache_id").toUInt())) {
                m_downloads.value(columnsMap.value("cache_id").toUInt())
                        ->removeLocation(columnsMap.value("client_id").toString());
            }
        }
    }
}

bool WebDownloadsManager::containsCacheLocation(uint cacheId, const QString &clientId) const
{
    if (!m_downloads.contains(cacheId))
        return false;

    return m_downloads.value(cacheId)->containsLocation(clientId);
}

bool WebDownloadsManager::isCacheAvailable(uint cacheId) const
{
    if (m_downloads.contains(cacheId))
        return m_downloads.value(cacheId)->isCacheAvailable();

    return false;
}

bool WebDownloadsManager::containsCacheExceptionFor(const QString &url)
{
    return m_cacheExceptions->containsExceptionFor(url);
}

WebDownloadsManager::WebDownloadsManager()
    : m_proxyPort(-1)
{
    m_gdsfClock = new GDSFClock;
    m_trafficCounter = new ProxyTrafficCounter;
    m_cacheExceptions = new CacheExceptions;
    m_cacheAccessManager = new CacheAccessManager;

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
    if (m_downloads.contains(cacheId)) {
        m_downloads.value(cacheId)->addLocation(clientId, dateCreated);
    } else {
        WebDownload *download = new WebDownload(cacheId);
        download->addLocation(clientId, dateCreated);
        m_downloads.insert(cacheId, download);
    }
}

bool WebDownloadsManager::shouldRefresh(ProxyRequest *request)
{
    if (request->domain() == "local")
        return false;

    return Session().isRefreshSession() || request->isRefreshRequest();
}


void WebDownloadsManager::logCacheAccess(uint cacheId, qint64 size, int numAccesses)
{
    m_cacheAccessManager->logCacheAccess(cacheId, size, numAccesses);
}

void WebDownloadsManager::saveCacheAccesses()
{
    m_cacheAccessManager->saveToDatabase();
}
