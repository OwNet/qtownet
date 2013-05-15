#include "proxywebreader.h"

ProxyWebReader::ProxyWebReader(SocketHandler *socketHandler, ProxyRequest *request, QObject *parent) :
    QObject(parent),
    m_socketHandler(socketHandler),
    m_request(request)
{
}

void ProxyWebReader::read()
{
}
