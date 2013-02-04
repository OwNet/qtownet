#ifndef PROXYSOCKET_H
#define PROXYSOCKET_H

#include <QObject>

#include "isocket.h"

class QTcpSocket;

class ProxySocket : public QObject, public ISocket
{
    Q_OBJECT
public:
    explicit ProxySocket(QObject *parent = 0);

    void setSocketDescriptor(int socketDescriptor);
    QIODevice *ioDevice();
    void disconnectFromHost();
    virtual bool waitForDisconnected(int msecs = 3000);
    bool flush();
    QAbstractSocket::SocketState state();

private:
    QTcpSocket *m_tcpSocket;
};

#endif // PROXYSOCKET_H
