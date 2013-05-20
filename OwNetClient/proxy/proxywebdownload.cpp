#include "proxywebdownload.h"

#include "proxyhandlersession.h"
#include "websocket.h"
#include "cachefolder.h"
#include "proxyrequest.h"
#include "proxywebreader.h"
#include "databasesettings.h"
#include "session.h"
#include "databaseupdatequery.h"
#include "synceddatabaseupdatequery.h"
#include "cachehelper.h"
#include "idatabaseselectquerywheregroup.h"
#include "webdownloadsmanager.h"
#include "gdsfclock.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QBuffer>

ProxyWebDownload::ProxyWebDownload(ProxyRequest *request, QObject *parent) :
    QObject(parent),
    m_request(request),
    m_started(false),
    m_failed(false),
    m_finished(false),
    m_type(WebStream),
    m_sessionDependentObjectId(-1),
    m_session(NULL)
{
    connect(this, SIGNAL(finished()), this, SLOT(deregisterDependentObject()));
    connect(this, SIGNAL(failed()), this, SLOT(deregisterDependentObject()));
    m_cacheId = request->hashCode();
}

ProxyWebDownload::ProxyWebDownload(uint cacheId, const QString &clientId, QObject *parent) :
    QObject(parent),
    m_request(NULL),
    m_started(false),
    m_failed(false),
    m_finished(false),
    m_type(NetworkStream),
    m_sessionDependentObjectId(-1),
    m_session(NULL),
    m_clientId(clientId),
    m_cacheId(cacheId)
{
    connect(this, SIGNAL(finished()), this, SLOT(deregisterDependentObject()));
    connect(this, SIGNAL(failed()), this, SLOT(deregisterDependentObject()));
    if (DatabaseSettings().clientId() == clientId) {
        m_type = CacheFile;
        m_started = true;
        m_finished = true;
    }
}

QIODevice *ProxyWebDownload::getStream(ProxyWebReader *reader, ProxyHandlerSession *session, bool *finished)
{
    *finished = false;
    m_startedMutex.lock();

    if (!m_failed && !m_finished) {
        connect(this, SIGNAL(readyRead()), reader, SLOT(readyRead()), Qt::QueuedConnection);
        connect(this, SIGNAL(finished()), reader, SLOT(finished()), Qt::QueuedConnection);
        connect(this, SIGNAL(failed()), reader, SLOT(failed()), Qt::QueuedConnection);

        if (!m_started) {
            m_session = session;
            startDownload();
            m_started = true;
        }
    } else {
        *finished = true;
    }

    m_startedMutex.unlock();

    QFile *file = NULL;
    if (!m_failed) {
        file = CacheFolder().cacheFile(m_cacheId, 0);
        file->open(QIODevice::ReadOnly);
        if (!file->exists()) {
            if (m_type == CacheFile) {
                downloadFromTheWebOrNetwork(m_request);
                return getStream(reader, session, finished);
            } else {
                return NULL;
            }
        }
    }

    return file;
}

bool ProxyWebDownload::exists()
{
    if (m_type == CacheFile) {
        QFile *file = CacheFolder().cacheFile(m_cacheId, 0);
        return file->exists();
    }
    return false;
}

void ProxyWebDownload::downloadFromTheWebOrNetwork(ProxyRequest *request, const QString clientId)
{
    m_startedMutex.lock();
    m_started = false;
    m_finished = false;
    m_failed = false;
    m_request = request;
    m_cacheId = request->hashCode();
    if (!clientId.isEmpty()) {
        m_type = NetworkStream;
        m_clientId = clientId;
    } else {
        m_type = WebStream;
    }
    m_startedMutex.unlock();
}

void ProxyWebDownload::startDownload()
{
    if (isWeb() || isNetworkCache()) {
        if (m_session)
            m_sessionDependentObjectId = m_session->registerDependentObject();

        CacheFolder cacheFolder;
        QFile *writeFile = cacheFolder.cacheFile(m_request, 0);
        if (writeFile->exists())
            writeFile->remove();
        writeFile->open(QIODevice::WriteOnly);

        WebSocket *socket = new WebSocket(m_request, writeFile);
        if (isNetworkCache() && !m_clientId.isEmpty()) {
            QVariantMap clients = Session().availableClients();
            if (clients.contains(m_clientId))
                socket->setProxy(clients.value(m_clientId).toString());
        }

        connect(socket, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
        connect(socket, SIGNAL(finished(qint64)), this, SLOT(downloadFinished(qint64)));
        connect(socket, SIGNAL(failed()), this, SLOT(downloadFailed()));
        socket->readRequest();

        m_type = CacheFile;
    }
}

void ProxyWebDownload::downloadFailed()
{
    m_startedMutex.lock();
    m_failed = true;
    emit failed();
    m_startedMutex.unlock();
}

void ProxyWebDownload::downloadFinished(qint64 size)
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
    m_finished = true;
    m_clientId = QString();
    emit finished();
    m_startedMutex.unlock();
}

void ProxyWebDownload::deregisterDependentObject()
{
    if (m_sessionDependentObjectId >= 0 && m_session) {
        m_session->deregisterDependentObject(m_sessionDependentObjectId);
        m_session = NULL;
        m_sessionDependentObjectId = -1;
    }
}

void ProxyWebDownload::saveToCache(uint hashCode, const QString &url, qint64 size, int numAccesses)
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
