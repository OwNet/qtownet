#include "servermodule.h"

#include "iproxyconnection.h"
#include "serverservice.h"
#include "clientservice.h"
#include "customserverservice.h"
#include "centralserviceservice.h"

void ServerModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;
    proxyConnection->registerService( new ServerService(proxyConnection,this) );
    proxyConnection->registerService( new ClientService(proxyConnection,this) );
    proxyConnection->registerService( new CustomServerService(proxyConnection,this) );
    proxyConnection->registerService( new CentralServiceService(proxyConnection,this) );
}
