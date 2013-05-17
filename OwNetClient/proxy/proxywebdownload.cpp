#include "proxywebdownload.h"

#include "proxyhandlersession.h"
#include "websocket.h"
#include "cachefolder.h"
#include "proxyrequest.h"
#include "proxywebreader.h"

#include <QBuffer>

ProxyWebDownload::ProxyWebDownload(ProxyRequest *request, QObject *parent) :
    QObject(parent),
    m_request(request),
    m_started(false),
    m_failed(false),
    m_type(WebStream),
    m_sessionDependentObjectId(-1),
    m_session(NULL)
{
    connect(this, SIGNAL(finished()), this, SLOT(deregisterDependentObject()));
}

QIODevice *ProxyWebDownload::getStream(ProxyWebReader *reader, ProxyHandlerSession *session)
{
    connect(this, SIGNAL(readyRead()), reader, SLOT(readyRead()));
    connect(this, SIGNAL(finished()), reader, SLOT(finished()));
    connect(this, SIGNAL(failed()), reader, SLOT(failed()));

    m_startedMutex.lock();
    if (m_failed)
        return NULL;

    if (!m_started) {
        m_session = session;
        m_sessionDependentObjectId = session->registerDependentObject();
        startDownload();
        m_started = true;
    }
    m_startedMutex.unlock();

    CacheFolder cacheFolder;
    QFile *file = cacheFolder.cacheFile(m_request, 0);
    file->open(QIODevice::ReadOnly);

    return file;
}

void ProxyWebDownload::startDownload()
{
    if (m_type == WebStream) {
        CacheFolder cacheFolder;
        QFile *writeFile = cacheFolder.cacheFile(m_request, 0, this);
        writeFile->open(QIODevice::WriteOnly);

        WebSocket *socket = new WebSocket(m_request, writeFile, this);
        connect(socket, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
        connect(socket, SIGNAL(finished()), this, SIGNAL(finished()));
        connect(socket, SIGNAL(failed()), this, SLOT(downloadFailed()));
        connect(socket, SIGNAL(finished()), socket, SLOT(deleteLater()));
        connect(socket, SIGNAL(failed()), socket, SLOT(deleteLater()));
        socket->readRequest();
    }
}

void ProxyWebDownload::downloadFailed()
{
    m_failed = true;
    emit failed();
}

void ProxyWebDownload::deregisterDependentObject()
{
    if (m_sessionDependentObjectId >= 0 && m_session) {
        m_session->deregisterDependentObject(m_sessionDependentObjectId);
        m_session = NULL;
        m_sessionDependentObjectId = -1;
    }
}
