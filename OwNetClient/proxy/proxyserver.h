#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QTcpServer>

class ProxyServer : public QTcpServer
{
public:
    ProxyServer(QObject *parent = 0);

    enum {
        PORT = 8081
    };

protected:
    void incomingConnection(int handle);
};

#endif // PROXYSERVER_H
