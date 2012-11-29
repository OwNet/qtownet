#include "communicationinitializer.h"
#include "heartbeatjob.h"
#include "communication/heartbeatserver.h"
#include "settings.h"
#include "messagehelper.h"

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

    MessageHelper::debug(QObject::tr("Proxy uses multicast address %1:%2")
                               .arg(groupAddress->toString()).arg(port));

    m_heartbeatJob = new HeartbeatJob(groupAddress, port);

    HeartbeatServer::getInstance()->start(groupAddress, port);
}
