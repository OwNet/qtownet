#ifndef PROXYSERVER_H
#define PROXYSERVER_H

#include <QTcpServer>

class TestServer : public QTcpServer
{
    Q_OBJECT

public:
    TestServer(QObject *parent = 0);

    enum {
        Port = 8091
    };

private slots:
    void readFromSocket();

protected:
    void incomingConnection(qintptr handle);
};

#endif // PROXYSERVER_H
