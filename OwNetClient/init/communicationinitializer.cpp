#include "communicationinitializer.h"
#include "heartbeatjob.h"
#include "communication/heartbeatserver.h"
#include "settings.h"

#include <QHostAddress>

CommunicationInitializer::CommunicationInitializer()
{
}

void CommunicationInitializer::init()
{
    QHostAddress *groupAddress = new QHostAddress(
                Settings().value("application/multicast_group_address", "227.227.227.1").toString()
    );
    int port = Settings().value("application/multicast_port", "8081").toInt();

    m_heartbeatJob = new HeartbeatJob(groupAddress, port);

    HeartbeatServer::getInstance()->start(groupAddress, port);
}
