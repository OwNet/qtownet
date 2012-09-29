#include "proxyserver.h"
#include "proxythread.h"

ProxyServer::ProxyServer(QObject *parent)
    : QTcpServer(parent)
{}

void ProxyServer::incomingConnection(int handle)
{
    ProxyThread *thread = new ProxyThread(handle, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
