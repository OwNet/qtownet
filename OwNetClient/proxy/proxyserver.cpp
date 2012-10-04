#include "proxyserver.h"
#include "proxythread.h"
#include "messagehelper.h"

ProxyServer::ProxyServer(QObject *parent)
    : QTcpServer(parent)
{}

void ProxyServer::incomingConnection(int handle)
{
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(handle);
}

void ProxyServer::readClient()
{
    QTcpSocket *socket = (QTcpSocket*)sender();
    ProxyThread *thread = new ProxyThread(socket, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void ProxyServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}
