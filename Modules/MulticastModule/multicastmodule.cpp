#include "multicastmodule.h"

#include "iproxyconnection.h"
#include "multicastserver.h"
#include "multicastprotocol.h"
#include "multicastjob.h"
#include "updatejob.h"
#include "multicastservice.h"

#include <QHostAddress>
#include <QSettings>

void MulticastModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;

    QHostAddress *groupAddress = new QHostAddress(
        proxyConnection->settings()->value("application/multicast_group_address",
                                           "227.227.227.1").toString()
    );
    int port = proxyConnection->settings()->value("application/multicast_port", 8081).toInt();

    m_multicastProtocol = new MulticastProtocol(proxyConnection, this);

    m_multicastServer = new MulticastServer(proxyConnection, m_multicastProtocol, this);
    m_multicastServer->start(groupAddress, port);

    proxyConnection->registerJob(new MulticastJob(groupAddress, port, m_multicastProtocol, m_proxyConnection, this));
    proxyConnection->registerJob(new UpdateJob(m_multicastProtocol, this));

    m_proxyConnection->registerService(new MulticastService(m_proxyConnection, m_multicastProtocol, this));
}
