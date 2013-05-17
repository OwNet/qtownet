#include "proxywebreader.h"

#include "proxywebdownload.h"
#include "sockethandler.h"
#include "proxyhandlersession.h"
#include "session.h"
#include "requestreader.h"

#include <QUrl>

ProxyWebReader::ProxyWebReader(SocketHandler *socketHandler, ProxyRequest *request, ProxyHandlerSession *session, QObject *parent) :
    QObject(parent),
    m_socketHandler(socketHandler),
    m_request(request),
    m_stream(NULL),
    m_session(session)
{
    m_dependantObjectId = m_session->registerDependentObject();
}

void ProxyWebReader::read()
{
    ProxyWebDownload *download = new ProxyWebDownload(m_request);
    m_stream = download->getStream(this, m_session);
}

void ProxyWebReader::readyRead()
{
    if (m_stream)
        m_socketHandler->write(m_stream->readAll());
    emit iAmActive();
}

void ProxyWebReader::finished()
{
    readyRead();
    m_session->deregisterDependentObject(m_dependantObjectId);
}

void ProxyWebReader::failed()
{
    if (!Session().isOnline()) {
        m_socketHandler->writeStatusCodeAndDescription(307, "Temporary Redirect");
        QVariantMap responseHeaders;
        responseHeaders.insert("Location", QString("http://my.ownet/?url=%1")
                               .arg(QString(QUrl::toPercentEncoding(m_socketHandler->requestReader()->url()))));
        responseHeaders.insert("Content-Type", "text/html");
        responseHeaders.insert("Content-Length", "0");
        m_socketHandler->writeHeaders(responseHeaders);
    }
}
