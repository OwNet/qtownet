#include "multicastjob.h"
#include "jsondocument.h"
#include "multicastprotocol.h"

#include <QUdpSocket>

MulticastJob::MulticastJob(QHostAddress *groupAddress, int port, QObject *parent)
    : m_groupAddress(groupAddress), m_port(port)
{
}

void MulticastJob::execute()
{
    // this should be done after first synchronisation
    /*
    MulticastProtocol::getInstance()->initialized();

    QUdpSocket *udpSocket = new QUdpSocket(this);

    QString status;
    switch (MulticastProtocol::getInstance()->myStatus())
    {
    case MulticastProtocol::INITIALIZING:
        status = "initializing";
        break;
    case MulticastProtocol::CLIENT:
        status = "client";
        break;
    case MulticastProtocol::SERVER:
        status = "server";
        break;
    }

    QVariantMap map;
    map.insert("id", MulticastProtocol::getInstance()->myId());
    map.insert("score", MulticastProtocol::getInstance()->myScore());
    map.insert("status", status);
    QByteArray datagram = JsonDocument::fromVariant(map).toJson();

    udpSocket->writeDatagram(datagram.data(), datagram.size(), *m_groupAddress, m_port);
    */
}
