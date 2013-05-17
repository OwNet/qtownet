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
    m_writeFile(NULL),
    m_socket(NULL)
{
}

QIODevice *ProxyWebDownload::getStream(ProxyWebReader *reader, ProxyHandlerSession *session)
{
    m_startedMutex.lock();
    if (m_failed)
        return NULL;

    if (!m_started) {
        m_sessionDependentObjectId = session->registerDependentObject();
        startDownload();
        m_started = true;
    }
    m_startedMutex.unlock();

    CacheFolder cacheFolder;
    m_writeFile = cacheFolder.cacheFile(m_request, 0);
    m_writeFile->open(QIODevice::ReadOnly);

    connect(this, SIGNAL(readyRead()), reader, SLOT(readyRead()));
    connect(this, SIGNAL(finished()), reader, SLOT(finished()));

    return m_writeFile;
}

void ProxyWebDownload::startDownload()
{
    if (m_type == WebStream) {
        CacheFolder cacheFolder;
        m_writeFile = cacheFolder.cacheFile(m_request, 0, this);
        m_writeFile->open(QIODevice::WriteOnly);

        m_socket = new WebSocket(m_request, m_writeFile, this);
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));
        connect(m_socket, SIGNAL(finished()), this, SLOT(downloadFinished()));
        connect(m_socket, SIGNAL(failed()), this, SLOT(downloadFailed()));
        m_socket->readRequest();
    }
}

void ProxyWebDownload::downloadReadyRead()
{
    m_writeFile->flush();
    emit readyRead();
}

void ProxyWebDownload::downloadFinished()
{
    if (m_writeFile) {
        m_writeFile->close();
        m_writeFile->deleteLater();
        m_writeFile = NULL;
    }
    if (m_socket) {
        m_socket->deleteLater();
        m_socket = NULL;
    }
    emit finished();
}

void ProxyWebDownload::downloadFailed()
{
    m_failed = true;
    if (m_writeFile) {
        m_writeFile->remove();
        m_writeFile->deleteLater();
        m_writeFile = NULL;
    }
    if (m_socket) {
        m_socket->deleteLater();
        m_socket = NULL;
    }
    emit finished();
}
