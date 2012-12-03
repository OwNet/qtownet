#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QTcpServer>

#include "proxyhandler.h"

class ProxyDownloads;

class ProxyServer : public QTcpServer
{
    Q_OBJECT

public:
    ProxyServer(QObject *parent = 0);

    enum {
        Port = 8081
    };

protected:
    void incomingConnection(qintptr handle);

private:
    ProxyHandler *initializeProxyHandler();

signals:
    void askAllHandlersToFinish();
};

#endif // PROXYSERVER_H
