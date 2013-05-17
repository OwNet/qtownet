#include "proxywebreader.h"

#include "proxywebdownload.h"
#include "sockethandler.h"
#include "proxyhandlersession.h"

ProxyWebReader::ProxyWebReader(SocketHandler *socketHandler, ProxyRequest *request, ProxyHandlerSession *session, QObject *parent) :
    QObject(parent),
    m_socketHandler(socketHandler),
    m_request(request),
    m_stream(NULL)
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
    m_session->deregisterDependentObject(m_dependantObjectId);
}
