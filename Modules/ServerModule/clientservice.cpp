#include "clientservice.h"

#include "irequest.h"
#include "clientservicecall.h"

#include <QStringList>

ClientService::ClientService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

IResponse *ClientService::processRequest(IBus *, IRequest *request)
{
    return request->response(ClientServiceCall(m_proxyConnection).callClientService(0, request));
}
