#include "serverservice.h"

#include "irequest.h"
#include "clientservicecall.h"

ServerService::ServerService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

QVariant *ServerService::processRequest(IBus *, IRequest *request)
{
    return ClientServiceCall(m_proxyConnection).callClientService(0, request);
}
