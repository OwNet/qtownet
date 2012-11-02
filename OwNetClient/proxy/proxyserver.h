#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QTcpServer>
#include <QQueue>
#include <QMutex>
#include <QMap>

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

private slots:
    void disposeHandlerIfNecessary(ProxyHandler *handler);
    void proxyRequestFinished(ProxyHandler * handler);

protected:
    void incomingConnection(int handle);

private:
    ProxyHandler *initializeProxyHandler();
    void disposeHandler(ProxyHandler *handler);

    QQueue<int> m_freeHandlerIds;
    QMap<int, ProxyHandler *> m_handlersMap;
    QMutex m_freeHandlersMutex;

    int m_lastHandlerId;

signals:
    void askAllHandlersToFinish();
    void handleNewConnection(int);
};

#endif // PROXYSERVER_H
