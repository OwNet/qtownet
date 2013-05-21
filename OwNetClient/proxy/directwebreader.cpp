#include "directwebreader.h"

#include "websocket.h"
#include "proxyhandlersession.h"
#include "websocketoutput.h"

DirectWebReader::DirectWebReader(SocketHandler *socketHandler, ProxyRequest *request, ProxyHandlerSession *session, QObject *parent) :
    QObject(parent),
    m_socketHandler(socketHandler),
    m_request(request),
    m_session(session)
{
    m_dependentObjectId = m_session->registerDependentObject();
}

void DirectWebReader::read()
{
    WebSocket *socket = new WebSocket(m_request, this, new WebSocketOutput(m_socketHandler));
    socket->readRequest();
}

void DirectWebReader::downloadFailed()
{
    close();
}

void DirectWebReader::downloadFinished(qint64 size)
{
    close();
}

void DirectWebReader::close()
{
    if (m_dependentObjectId >= 0)
        m_session->deregisterDependentObject(m_dependentObjectId);
    m_dependentObjectId = -1;
}
