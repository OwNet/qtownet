#ifndef PROXYSOCKETREQUEST_H
#define PROXYSOCKETREQUEST_H

#include "proxyrequest.h"

class QTcpSocket;
class QIODevice;

/**
 * @brief ProxyRequest that is able to read the request from socket
 */
class ProxySocketRequest : public ProxyRequest
{
public:
    ProxySocketRequest(QTcpSocket *socket, QObject *parent = 0);
    ProxySocketRequest(QIODevice *socketIODevice, QObject *parent = 0);

    bool readFromSocket();
    QIODevice *socket() const;

    QString peerAddress() const;
    quint16 peerPort() const;

private:
    QTcpSocket *m_socket;
    QIODevice *m_socketIODevice;
};

#endif // PROXYSOCKETREQUEST_H
