#include "clientservice.h"

#include "irequest.h"
#include "clientservicecall.h"

#include <QStringList>

ClientService::ClientService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

QVariant *ClientService::processRequest(IBus *, IRequest *request)
{
    return ClientServiceCall(m_proxyConnection).callClientService(request->id(), request);
}
