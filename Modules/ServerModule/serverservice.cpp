#include "serverservice.h"

#include "irequest.h"
#include "clientservicecall.h"
#include "irouter.h"

ServerService::ServerService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

IResponse *ServerService::processRequest(IBus *, IRequest *request)
{
    return request->response(ClientServiceCall(m_proxyConnection).callClientService(0, request));
}
