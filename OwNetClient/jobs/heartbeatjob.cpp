#include "heartbeatjob.h"
#include "messagehelper.h"
#include "jsondocument.h"
#include "communication/communicationmanager.h"

#include <QUdpSocket>

HeartbeatJob::HeartbeatJob(QHostAddress *groupAddress, int port, QObject *parent)
    : Job(5 * 1000, parent), m_groupAddress(groupAddress), m_port(port)
{
}

void HeartbeatJob::execute()
{
    // this should be done after first synchronisation
    CommunicationManager::getInstance()->initialized();

    QUdpSocket *udpSocket = new QUdpSocket(this);

    QString status;
    switch (CommunicationManager::getInstance()->myStatus())
    {
    case CommunicationManager::INITIALIZING:
        status = "initializing";
        break;
    case CommunicationManager::CLIENT:
        status = "client";
        break;
    case CommunicationManager::SERVER:
        status = "server";
        break;
    }

    QVariantMap map;
    map.insert("id", CommunicationManager::getInstance()->myId());
    map.insert("score", CommunicationManager::getInstance()->myScore());
    map.insert("status", status);
    QByteArray datagram = JsonDocument::fromVariant(map).toJson();

    udpSocket->writeDatagram(datagram.data(), datagram.size(), *m_groupAddress, m_port);
}
