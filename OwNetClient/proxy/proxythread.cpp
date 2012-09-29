#include "proxythread.h"

#include "messagehelper.h"

ProxyThread::ProxyThread(int socketDescriptor, QObject *parent)
    : QThread(parent), m_socketDescriptor(socketDescriptor)
{
}

void ProxyThread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(m_socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }
    if (tcpSocket.isReadable() && tcpSocket.isWritable()) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out << "It works!";

        tcpSocket.write(block);
    }

    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}
