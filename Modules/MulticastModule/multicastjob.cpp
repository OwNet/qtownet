#include "multicastjob.h"

#include "multicastprotocol.h"
#include "iproxyconnection.h"

#include <QUdpSocket>

MulticastJob::MulticastJob(QHostAddress *groupAddress, int port,  MulticastProtocol *protocol,
                           IProxyConnection *proxyConnection, QObject *parent)
    : m_groupAddress(groupAddress),
      m_protocol(protocol),
      m_port(port),
      m_proxyConnection(proxyConnection),
      QObject(parent)
{
    // this should be done after first synchronisation
    m_protocol->initialized();
}

void MulticastJob::execute()
{
    m_protocol->update();

    QUdpSocket *udpSocket = new QUdpSocket(this);
    QByteArray datagram = m_proxyConnection->toJson(m_protocol->message());
    udpSocket->writeDatagram(datagram.data(), datagram.size(), *m_groupAddress, m_port);
}
