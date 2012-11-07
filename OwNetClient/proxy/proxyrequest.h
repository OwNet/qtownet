#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include "listofstringpairs.h"

#include <QNetworkRequest>
#include <QMap>
#include <QByteArray>

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
    ListOfStringPairs &requestHeaders() { return m_requestHeaders; }

    const QString &url() { return m_url; }
    const QString requestContentType();
    const QString &relativeUrl() { return m_relativeUrl; }
    const QString &action() { return m_action; }
    const QString &module() { return m_module; }
    const int &id() { return m_id; }
    const QMap<QString, QString> &parameters() { return m_parameters; }
    const QByteArray &requestBody() { return m_requestBody; }

    int &hashCode() { return m_hashCode; }

    bool isLocalRequest() { return m_domain == "ownet"; }
    bool isStaticResourceRequest() { return m_domain == "ownet" && m_subDomain == "static"; }

    QTcpSocket *socket() { return m_socket; }

private:
    const QString urlExtension();
    void analyzeUrl();
    static QMap<QString, QString> initContentTypes();

    QString m_requestMethod;
    QString m_relativeUrl;
    QString m_domain;
    QString m_subDomain;
    QString m_url;
    QString m_module;
    QString m_action;
    int m_id;
    QMap<QString, QString> m_parameters;
    QByteArray m_requestBody;

    QTcpSocket *m_socket;
    static QMap<QString, QString> m_contentTypes;
    ListOfStringPairs m_requestHeaders;
    int m_hashCode;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
