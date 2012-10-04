#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QTcpServer>

class ProxyServer : public QTcpServer
{
    Q_OBJECT

public:
    ProxyServer(QObject *parent = 0);

    enum {
        PORT = 8081
    };

protected:
    void incomingConnection(int handle);

private slots:
    void readClient();
    void discardClient();
};

#endif // PROXYSERVER_H
