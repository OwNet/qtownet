#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QTcpServer>
#include <QQueue>
#include <QMutex>

#include "proxyhandler.h"

class ProxyServer : public QTcpServer
{
    Q_OBJECT

public:
    ProxyServer(QObject *parent = 0);

    enum {
        PORT = 8081,
        INITIAL_NUM_PROXY_HANDLERS = 5,
        MAX_NUM_PROXY_HANDLERS = 15
    };

protected:
    void incomingConnection(int handle);

private:
    ProxyHandler *initializeProxyHandler();

    QQueue<ProxyHandler *> m_freeHandlers;
    QMutex m_freeHandlersMutex;

signals:
    void askAllHandlersToFinish();
    void handleNewConnection(int);

private slots:
    void proxyRequestFinished(ProxyHandler * handler);
};

#endif // PROXYSERVER_H
