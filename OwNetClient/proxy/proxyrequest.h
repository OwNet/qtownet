#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include <QString>
#include <QNetworkRequest>

class QTcpSocket;

class ProxyRequest
{
public:
    enum RequestType {
        GET,
        POST,
        PUT,
        DELETE,
        UNKNOWN
    };

    ProxyRequest(QTcpSocket *socket);
    ~ProxyRequest();

    bool readFromSocket();
    ProxyRequest::RequestType requestType();
    const QNetworkRequest &networkRequest() { return m_networkRequest; }

private:
    QTcpSocket *m_socket;
    QNetworkRequest m_networkRequest;
    QString m_requestMethod;
};

#endif // PROXYREQUEST_H
