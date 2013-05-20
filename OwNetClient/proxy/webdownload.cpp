#include "webdownload.h"

#include "proxyhandlersession.h"
#include "websocket.h"
#include "cachefolder.h"
#include "proxyrequest.h"
#include "webreader.h"
#include "databasesettings.h"
#include "session.h"
#include "databaseupdatequery.h"
#include "synceddatabaseupdatequery.h"
#include "cachehelper.h"
#include "idatabaseselectquerywheregroup.h"
#include "webdownloadsmanager.h"
#include "gdsfclock.h"
#include "websocketoutput.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QBuffer>

WebDownload::WebDownload(uint cacheId, QObject *parent) :
    QObject(parent),
    m_request(NULL),
    m_inProgress(false),
    m_sessionDependentObjectId(-1),
    m_session(NULL),
    m_cacheId(cacheId)
{
    m_cacheLocations.setCacheId(m_cacheId);
}

QIODevice *WebDownload::getStream(ProxyRequest *request, WebReader *reader, ProxyHandlerSession *session, bool refresh, bool *finished)
{
    m_request = request;
    *finished = false;

    m_startedMutex.lock();

    if (m_inProgress) {
        connect(this, SIGNAL(readyRead()), reader, SLOT(readyRead()), Qt::QueuedConnection);
        connect(this, SIGNAL(finished()), reader, SLOT(finished()), Qt::QueuedConnection);
        connect(this, SIGNAL(failed()), reader, SLOT(failed()), Qt::QueuedConnection);
    } else {
        QPair<CacheLocations::LocationType, QString> locationType = m_cacheLocations.getLocationType(refresh);
        if (locationType.first == CacheLocations::LocalCache) {
            *finished = true;
        } else {
            connect(this, SIGNAL(readyRead()), reader, SLOT(readyRead()), Qt::QueuedConnection);
            connect(this, SIGNAL(finished()), reader, SLOT(finished()), Qt::QueuedConnection);
            connect(this, SIGNAL(failed()), reader, SLOT(failed()), Qt::QueuedConnection);

            m_session = session;
            m_sessionDependentObjectId = m_session->registerDependentObject();
            startDownload(locationType.first, locationType.second);
            m_inProgress = true;
        }
    }

    m_startedMutex.unlock();

    QFile *file = CacheFolder().cacheFile(m_cacheId, 0);
    if (!file->exists())
        return NULL;
    file->open(QIODevice::ReadOnly);

    return file;
}

void WebDownload::startDownload(CacheLocations::LocationType locationType, QString clientId)
{
    CacheFolder cacheFolder;
    QFile *writeFile = cacheFolder.cacheFile(m_cacheId, 0);
    if (writeFile->exists())
        writeFile->remove();
    writeFile->open(QIODevice::WriteOnly);

    WebSocket *socket = new WebSocket(m_request, this, new WebSocketOutput(writeFile));
    if (locationType == CacheLocations::NetworkCache) {
        QVariantMap clients = Session().availableClients();
        if (clients.contains(clientId))
            socket->setProxy(clients.value(clientId).toString());
    }

    connect(socket, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
    socket->readRequest();
}

void WebDownload::downloadFailed()
{
    m_startedMutex.lock();
    m_inProgress = false;
    m_cacheLocations.removeLocalLocation();
    emit failed();
    m_startedMutex.unlock();

    deregisterDependentObject();
}

void WebDownload::downloadFinished(qint64 size)
{
    QString url = m_request->url();

    QSqlQuery sqlQuery;
    sqlQuery.prepare("SELECT access_count FROM caches WHERE id = :id LIMIT 1");
    sqlQuery.bindValue(":id", m_cacheId);
    int accessCount = 0;

    if (sqlQuery.exec() && sqlQuery.next())
        accessCount = sqlQuery.value(sqlQuery.record().indexOf("access_count")).toInt();
    accessCount++;

    saveToCache(m_cacheId, url, size, accessCount);

    m_startedMutex.lock();
    m_inProgress = false;
    m_cacheLocations.addLocalLocation();
    emit finished();
    m_startedMutex.unlock();

    deregisterDependentObject();
}

void WebDownload::deregisterDependentObject()
{
    if (m_sessionDependentObjectId >= 0 && m_session) {
        m_session->deregisterDependentObject(m_sessionDependentObjectId);
        m_session = NULL;
        m_sessionDependentObjectId = -1;
    }
}

void WebDownload::saveToCache(uint hashCode, const QString &url, qint64 size, int numAccesses)
{
    if (!CacheHelper::canUseDatabase())
        return;
    {
        DatabaseUpdateQuery query("caches", DatabaseUpdateQuery::InsertOrUpdate);
        query.setUpdateDates(true);
        query.singleWhere("id", hashCode);
        query.setColumnValue("id", hashCode);
        query.setColumnValue("absolute_uri", url);
        query.setColumnValue("size", size);
        query.setColumnValue("access_value", WebDownloadsManager::instance()->gdsfClock()->getGDSFPriority(numAccesses, size));
        query.executeQuery();
    }
    {
        QString clientId = DatabaseSettings().clientId();
        SyncedDatabaseUpdateQuery query("client_caches");
        query.setUpdateDates(IDatabaseUpdateQuery::DateCreated);
        query.setColumnValue("client_id", clientId);
        query.setColumnValue("cache_id", hashCode);
        IDatabaseSelectQueryWhereGroup *where = query.whereGroup(IDatabaseSelectQuery::And);
        where->where("client_id", clientId);
        where->where("cache_id", hashCode);
        query.setGroupId(ISyncedDatabaseUpdateQuery::GroupCaches);
        query.setForceOperation(WebDownloadsManager::instance()->containsCacheLocation(hashCode, clientId) ?
                                    DatabaseUpdateQuery::ForceUpdate : DatabaseUpdateQuery::ForceInsert);
        query.executeQuery();
    }
}
