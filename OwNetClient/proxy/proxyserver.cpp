#include "proxyserver.h"
#include "messagehelper.h"

#include "proxydownloads.h"

ProxyServer::ProxyServer(QObject *parent)
    : QTcpServer(parent), m_lastHandlerId(0)
{
    for (int i = 0; i < InitialNumberOfProxyHandlers; ++i)
        m_freeHandlerIds.enqueue(initializeProxyHandler()->handlerId());
}

ProxyHandler * ProxyServer::initializeProxyHandler()
{
    QThread *t = new QThread();

    ProxyHandler *handler = new ProxyHandler(m_lastHandlerId++, t);
    m_handlersMap.insert(handler->handlerId(), handler);
    connect(this, SIGNAL(askAllHandlersToFinish()), handler, SLOT(finishHandling()));

    t->start();
    t->moveToThread(t);
    handler->moveToThread(t);

    connect(handler, SIGNAL(disposeThread()), t, SLOT(quit()));
    //connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(handler, SIGNAL(requestFinished(ProxyHandler *)), this, SLOT(proxyRequestFinished(ProxyHandler *)));

    return handler;
}

void ProxyServer::incomingConnection(int handle)
{
    ProxyHandler * handler = NULL;

    m_freeHandlersMutex.lock();
    while (!handler && !m_freeHandlerIds.isEmpty()) {
        int handlerId = m_freeHandlerIds.dequeue();
        if (m_handlersMap.contains(handlerId))
            handler = m_handlersMap.value(handlerId);
    }
    m_freeHandlersMutex.unlock();

    if (!handler)
        handler = initializeProxyHandler();

    handler->setDescriptorAndStart(handle);
}

void ProxyServer::proxyRequestFinished(ProxyHandler *handler) {
    m_freeHandlersMutex.lock();

    /*if (m_freeHandlerIds.count() > MaxNumberOfProxyHandlers) {
        m_handlersMap.remove(handler->handlerId());*/
        handler->dispose();
    /*} else {
        m_freeHandlerIds.enqueue(handler->handlerId());
    }*/

    m_freeHandlersMutex.unlock();
}
