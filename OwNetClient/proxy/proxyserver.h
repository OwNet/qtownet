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
    void proxyRequestFinished(ProxyHandler * handler);

protected:
    void incomingConnection(qintptr handle);

private:
    ProxyHandler *initializeProxyHandler();

    QQueue<int> m_freeHandlerIds;
    QMap<int, ProxyHandler *> m_handlersMap;
    QMutex m_freeHandlersMutex;

    int m_lastHandlerId;

signals:
    void askAllHandlersToFinish();
    void handleNewConnection(int);
};

#endif // PROXYSERVER_H
