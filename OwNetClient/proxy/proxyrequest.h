#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include "listofstringpairs.h"

#include <QNetworkRequest>
#include <QMap>

class QTcpSocket;

class ProxyRequest : public QObject
{
    Q_OBJECT

public:
    enum RequestType {
        GET,
        POST,
        PUT,
        DELETE,
        UNKNOWN
    };

    const QString &subDomain(){ return m_subDomain;}
    ProxyRequest(QTcpSocket *socket, QObject *parent = 0);

    bool readFromSocket();
    ProxyRequest::RequestType requestType();
    const ListOfStringPairs &requestHeaders() { return m_requestHeaders; }
    const QString url() { return m_url; }
    const QString requestContentType();
    const QString relativeUrl() { return m_relativeUrl; }
    bool isLocalRequest() { return m_domain == "ownet"; }
    bool isStaticResourceRequest() { return m_domain == "ownet" && m_subDomain == "static"; }

private:
    const QString urlExtension();
    void analyzeUrl();
    static QMap<QString, QString> initContentTypes();

    static QMap<QString, QString> m_contentTypes;

    QTcpSocket *m_socket;

    QString m_requestMethod;
    QString m_relativeUrl;
    QString m_domain;
    QString m_subDomain;
    QString m_url;

    ListOfStringPairs m_requestHeaders;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
