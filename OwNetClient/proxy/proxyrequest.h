#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include <QString>
#include <QNetworkRequest>
#include <QMap>

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
    const QString relativeUrl();
    const QString url();
    const QString requestContentType();

private:
    const QString urlExtension();
    static QMap<QString, QString> initContentTypes();

    static QMap<QString, QString> m_contentTypes;

    QTcpSocket *m_socket;
    QNetworkRequest m_networkRequest;
    QString m_requestMethod;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
