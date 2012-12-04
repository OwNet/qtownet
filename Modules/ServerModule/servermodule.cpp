#include "servermodule.h"

#include "serverservice.h"
#include "clientservice.h"

QList<IRestService *> *ServerModule::restServices()
{
    QList<IRestService *> *list = new QList<IRestService *>;
    list->append(new ServerService(m_proxyConnection, this));
    list->append(new ClientService(m_proxyConnection, this));
    return list;
}


void ServerModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;
}
