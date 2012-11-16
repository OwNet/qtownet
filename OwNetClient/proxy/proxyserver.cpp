#include "proxyserver.h"
#include "messagehelper.h"

#include "proxydownloads.h"

ProxyServer::ProxyServer(QObject *parent)
    : QTcpServer(parent), m_lastHandlerId(0)
{
    for (int i = 0; i < InitialNumberOfProxyHandlers; ++i)
        m_freeHandlerIds.enqueue(initializeProxyHandler()->handlerId());
}

void ProxyServer::disposeHandlerIfNecessary(ProxyHandler *handler)
{
    m_freeHandlersMutex.lock();
    if (m_freeHandlerIds.count() > MaxNumberOfProxyHandlers) {
        if (!handler->isActive())
           disposeHandler(handler);
    }
    m_freeHandlersMutex.unlock();
}

ProxyHandler * ProxyServer::initializeProxyHandler()
{
    ProxyHandler *handler = new ProxyHandler(m_lastHandlerId++);
    m_handlersMap.insert(handler->handlerId(), handler);
    connect(this, SIGNAL(askAllHandlersToFinish()), handler, SLOT(finishHandling()));
    connect(handler, SIGNAL(canBeDisposed(ProxyHandler*)), this, SLOT(disposeHandlerIfNecessary(ProxyHandler*)));

    QThread *t = new QThread();
    t->start();
    t->moveToThread(t);
    handler->moveToThread(t);

    connect(handler, SIGNAL(finished()), t, SLOT(quit()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(handler, SIGNAL(requestFinished(ProxyHandler *)), this, SLOT(proxyRequestFinished(ProxyHandler *)));

    return handler;
}

void ProxyServer::disposeHandler(ProxyHandler *handler)
{
    m_handlersMap.remove(handler->handlerId());
    handler->triggerFinish();
    delete handler;
}

void ProxyServer::incomingConnection(qintptr handle)
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

    if (m_freeHandlerIds.count() > MaxNumberOfProxyHandlers && !handler->hasDependentObjects()) {
        disposeHandler(handler);
    } else {
        m_freeHandlerIds.enqueue(handler->handlerId());
    }

    m_freeHandlersMutex.unlock();
}
