#include "webreader.h"

#include "webdownload.h"
#include "sockethandler.h"
#include "proxyhandlersession.h"
#include "session.h"
#include "requestreader.h"
#include "webdownloadsmanager.h"

#include <QUrl>

WebReader::WebReader(SocketHandler *socketHandler, ProxyRequest *request, ProxyHandlerSession *session, QObject *parent) :
    QObject(parent),
    m_socketHandler(socketHandler),
    m_request(request),
    m_stream(NULL),
    m_session(session),
    m_writtenToSocket(false)
{
    m_dependentObjectId = m_session->registerDependentObject();
}

void WebReader::read()
{
    bool done = false;

    m_readMutex.lock();
    m_stream = WebDownloadsManager::instance()->getStream(m_request, this, m_session, &done);
    m_readMutex.unlock();

    if (!m_stream)
        failed();
    else if (done)
        finished();
}

void WebReader::readyRead()
{
    m_readMutex.lock();
    if (m_stream) {
        QByteArray bytes = m_stream->readAll();
        if (!bytes.isEmpty())
            m_socketHandler->write(bytes);
    }
    m_readMutex.unlock();
    m_writtenToSocket = true;

    emit iAmActive();
}

void WebReader::finished()
{
    readyRead();
    if (m_dependentObjectId >= 0)
        m_session->deregisterDependentObject(m_dependentObjectId);
    m_dependentObjectId = -1;
}

void WebReader::failed()
{
    if (!m_writtenToSocket) {
        QVariantMap responseHeaders;
        if (!Session().isOnline()) {
            m_socketHandler->writeStatusCodeAndDescription(307, "Temporary Redirect");
            responseHeaders.insert("Location", QString("http://my.ownet/?url=%1")
                                   .arg(QString(QUrl::toPercentEncoding(m_socketHandler->requestReader()->url()))));
            responseHeaders.insert("Content-Type", "text/html");
        } else {
            m_socketHandler->writeStatusCodeAndDescription(418, "I'm a teapot");
        }
        responseHeaders.insert("Content-Length", "0");
        m_socketHandler->writeHeaders(responseHeaders);
        m_socketHandler->write(QByteArray());
    }
    if (m_dependentObjectId >= 0)
        m_session->deregisterDependentObject(m_dependentObjectId);
    m_dependentObjectId = -1;
}
