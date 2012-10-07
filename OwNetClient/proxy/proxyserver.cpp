#include "proxyserver.h"
#include "messagehelper.h"

ProxyServer::ProxyServer(QObject *parent)
    : QTcpServer(parent)
{
    for (int i = 0; i < INITIAL_NUM_PROXY_HANDLERS; ++i)
        m_freeHandlers.enqueue(initializeProxyHandler());
}

ProxyHandler * ProxyServer::initializeProxyHandler()
{
    ProxyHandler *handler = new ProxyHandler();
    connect(this, SIGNAL(askAllHandlersToFinish()), handler, SLOT(finish()));

    QThread *t = new QThread();
    handler->moveToThread(t);

    connect(handler, SIGNAL(finished()), t, SLOT(quit()));
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(handler, SIGNAL(requestFinished(ProxyHandler *)), this, SLOT(proxyRequestFinished(ProxyHandler *)));
    t->start();

    return handler;
}

void ProxyServer::incomingConnection(int handle)
{
    ProxyHandler * handler = NULL;

    m_freeHandlersMutex.lock();
    if (!m_freeHandlers.isEmpty())
        handler = m_freeHandlers.dequeue();
    m_freeHandlersMutex.unlock();

    if (handler == NULL) {
        handler = initializeProxyHandler();
        MessageHelper::debug("No thread left");
    }

    handler->setDescriptorAndStart(handle);
}

void ProxyServer::proxyRequestFinished(ProxyHandler * handler) {
    bool enqueued = false;

    m_freeHandlersMutex.lock();
    if (m_freeHandlers.count() < MAX_NUM_PROXY_HANDLERS) {
        m_freeHandlers.enqueue(handler);
        enqueued = true;
    }
    m_freeHandlersMutex.unlock();

    if (!enqueued) {
        MessageHelper::debug(QString("+++++ Deleting thread %1").arg(m_freeHandlers.count()));
        handler->finish();
        delete handler;
    }
}
