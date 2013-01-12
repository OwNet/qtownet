#include "proxysocket.h"

#include <QTcpSocket>

ProxySocket::ProxySocket(QObject *parent) :
    QObject(parent)
{
    m_tcpSocket = new QTcpSocket(this);
}

void ProxySocket::setSocketDescriptor(int socketDescriptor)
{
    m_tcpSocket->setSocketDescriptor(socketDescriptor);
}

QIODevice *ProxySocket::ioDevice()
{
    return m_tcpSocket;
}

void ProxySocket::disconnectFromHost()
{
    m_tcpSocket->disconnectFromHost();
}

bool ProxySocket::waitForDisconnected(int msecs)
{
    m_tcpSocket->waitForDisconnected(msecs);
}

bool ProxySocket::flush()
{
    m_tcpSocket->flush();
}

QAbstractSocket::SocketState ProxySocket::state()
{
    return m_tcpSocket->state();
}
