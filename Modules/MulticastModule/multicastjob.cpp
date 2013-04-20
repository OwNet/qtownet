#include "multicastjob.h"

#include "multicastprotocol.h"
#include "iproxyconnection.h"

#include <QUdpSocket>

MulticastJob::MulticastJob(QHostAddress *groupAddress, int port)
    : m_groupAddress(groupAddress),
      m_protocol(NULL),
      m_port(port),
      m_proxyConnection(NULL)
{
}

void MulticastJob::execute()
{
    m_protocol->update();

    QUdpSocket *udpSocket = new QUdpSocket(this);
    QByteArray datagram = m_proxyConnection->toJson(m_protocol->message());
    udpSocket->writeDatagram(datagram.data(), datagram.size(), *m_groupAddress, m_port);
}

void MulticastJob::setProxyConnection(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;
    m_protocol = new MulticastProtocol(m_proxyConnection, this);
    // this should be done after first synchronisation
    m_protocol->initialized();
}
