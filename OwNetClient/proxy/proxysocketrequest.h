#ifndef PROXYSOCKETREQUEST_H
#define PROXYSOCKETREQUEST_H

#include "proxyrequest.h"

/**
 * @brief ProxyRequest that is able to read the request from socket
 */
class ProxySocketRequest : public ProxyRequest
{
public:
    ProxySocketRequest(QIODevice *socket, QObject *parent = 0);

    bool readFromSocket();
    QIODevice *socket() const { return m_socket; }

private:
    QIODevice *m_socket;
};

#endif // PROXYSOCKETREQUEST_H
