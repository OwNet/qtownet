#include "multicastserver.h"
#include "multicastprotocol.h"
#include "jsondocument.h"
#include "iproxyconnection.h"

#include <QHostAddress>
#include <QUdpSocket>
#include <QVariantMap>

MulticastServer::MulticastServer(IProxyConnection *connection, MulticastProtocol *protocol,
                                 QObject *parent)
    : m_proxyConnection(connection), m_protocol(protocol), QObject(parent)
{
}

void MulticastServer::start(QHostAddress *groupAddress, int port)
{
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    m_udpSocket->joinMulticastGroup(*groupAddress);

    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void MulticastServer::stop()
{
    m_udpSocket->close();
}

void MulticastServer::processPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;

        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        QJsonParseError ok;
        JsonDocument json = JsonDocument::fromJson(datagram.data(),&ok);
        if (ok.error == QJsonParseError::NoError ) {
           QVariantMap result = json.object().toVariantMap();
           m_protocol->processMessage(&result);
        }
    }
}
