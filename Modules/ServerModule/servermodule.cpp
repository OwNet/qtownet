#include "servermodule.h"

#include "iproxyconnection.h"
#include "serverservice.h"
#include "clientservice.h"

void ServerModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;
    proxyConnection->registerRestService( new ServerService(proxyConnection,this) );
    proxyConnection->registerRestService( new ClientService(proxyConnection,this) );
}
