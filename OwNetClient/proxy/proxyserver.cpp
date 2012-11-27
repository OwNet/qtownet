#include "proxyserver.h"
#include "messagehelper.h"

#include "proxydownloads.h"

ProxyServer::ProxyServer(QObject *parent)
    : QTcpServer(parent)
{
}

/**
 * @brief Create a new instance of ProxyHandler in a new QThread.
 * @return New ProxyHandler instance
 */
ProxyHandler * ProxyServer::initializeProxyHandler()
{
    QThread *t = new QThread;

    ProxyHandler *handler = new ProxyHandler(t);
    connect(this, SIGNAL(askAllHandlersToFinish()), handler, SLOT(finishHandling()));

    t->start();
    t->moveToThread(t);
    handler->moveToThread(t);

    connect(handler, SIGNAL(disposeThread()), t, SLOT(quit()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()), Qt::QueuedConnection);

    return handler;
}

/**
 * @brief Received new open socket with request.
 * @param handle Socket descriptor
 */
void ProxyServer::incomingConnection(int handle)
{
    initializeProxyHandler()->setDescriptorAndStart(handle);
}
