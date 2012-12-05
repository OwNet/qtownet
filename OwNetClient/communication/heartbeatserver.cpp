#include "heartbeatserver.h"
#include "messagehelper.h"
#include "qjson/parser.h"
#include "communicationmanager.h"

#include <QHostAddress>
#include <QUdpSocket>
#include <QVariantMap>

HeartbeatServer *HeartbeatServer::m_heartbeatServer = NULL;

HeartbeatServer *HeartbeatServer::getInstance()
{
    if (! m_heartbeatServer)
        m_heartbeatServer = new HeartbeatServer();

    return m_heartbeatServer;
}

HeartbeatServer::HeartbeatServer()
{
}

void HeartbeatServer::start(QHostAddress *groupAddress, int port)
{
     m_udpSocket = new QUdpSocket(this);
     m_udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
     m_udpSocket->joinMulticastGroup(*groupAddress);

     connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void HeartbeatServer::stop()
{
    m_udpSocket->close();
}

void HeartbeatServer::processPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams()) {
         QByteArray datagram;

         datagram.resize(m_udpSocket->pendingDatagramSize());
         m_udpSocket->readDatagram(datagram.data(), datagram.size());

         QJson::Parser parser;
         bool ok;

         // MessageHelper::debug(datagram.data());

         QVariantMap result = parser.parse(datagram.data(), &ok).toMap();
         if (ok)
            CommunicationManager::getInstance()->processMessage(&result);
     }
}
