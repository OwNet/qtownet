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
        Port = 8081
    };

protected:
    void incomingConnection(int handle);

private:
    ProxyHandler *initializeProxyHandler();

signals:
    void askAllHandlersToFinish();
};

#endif // PROXYSERVER_H
