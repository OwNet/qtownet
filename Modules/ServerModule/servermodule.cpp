#include "servermodule.h"

#include "iproxyconnection.h"
#include "serverservice.h"
#include "clientservice.h"
#include "multicastserver.h"
#include "multicastjob.h"

#include <QHostAddress>
#include <QSettings>

void ServerModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;
    proxyConnection->registerService( new ServerService(proxyConnection,this) );
    proxyConnection->registerService( new ClientService(proxyConnection,this) );

    QHostAddress *groupAddress = new QHostAddress(
                proxyConnection->settings()->value("application/multicast_group_address", "227.227.227.1").toString()
    );
    int port = proxyConnection->settings()->value("application/multicast_port", "8081").toInt();

    m_multicastServer = new MulticastServer(proxyConnection);
    m_multicastServer->start(groupAddress, port);

    proxyConnection->registerJob(new MulticastJob(groupAddress, port, this));
}
