#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QTcpServer>
#include <QQueue>
#include <QMutex>

#include "proxyhandler.h"

class ProxyDownloads;

class ProxyServer : public QTcpServer
{
    Q_OBJECT

public:
    ProxyServer(QObject *parent = 0);

    enum {
        Port = 8081,
        InitialNumberOfProxyHandlers = 5,
        MaxNumberOfProxyHandlers = 15
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
