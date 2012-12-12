#include "multicastjob.h"
#include "jsondocument.h"
#include "multicastprotocol.h"

#include <QUdpSocket>

MulticastJob::MulticastJob(QHostAddress *groupAddress, int port,  MulticastProtocol *protocol,
                           QObject *parent)
    : m_groupAddress(groupAddress), m_protocol(protocol), m_port(port)
{
}

void MulticastJob::execute()
{
    // this should be done after first synchronisation
    m_protocol->initialized();

    QUdpSocket *udpSocket = new QUdpSocket(this);
    QByteArray datagram = JsonDocument::fromVariant(m_protocol->getMessage()).toJson();
    udpSocket->writeDatagram(datagram.data(), datagram.size(), *m_groupAddress, m_port);
}
