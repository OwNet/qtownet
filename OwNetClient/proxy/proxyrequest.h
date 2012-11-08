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

    ProxyRequest(QTcpSocket *socket, QObject *parent = 0);

    bool readFromSocket();
    ProxyRequest::RequestType requestType();
    ListOfStringPairs &requestHeaders() { return m_requestHeaders; }

    QString url() const { return m_url; }
    QString requestContentType() const;
    QString relativeUrl() const { return m_relativeUrl; }
    QString action() const { return m_action; }
    QString module() const { return isLocalRequest() ? m_module : QString(); }
    QString subDomain() const { return m_subDomain; }

    int id() const { return m_id; }
    QMap<QString, QString> parameters() const { return m_parameters; }
    QByteArray &requestBody() { return m_requestBody; }
    QString staticResourcePath() const;

    int hashCode() const { return m_hashCode; }

    bool isLocalRequest() const { return m_domain == "ownet"; }
    bool isStaticResourceRequest() const;
    bool isApiRequst() const { return m_isApiRequest; }

    QTcpSocket *socket() { return m_socket; }

private:
    QString urlExtension() const;
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
    bool m_isApiRequest;

    QTcpSocket *m_socket;
    static QMap<QString, QString> m_contentTypes;
    ListOfStringPairs m_requestHeaders;
    int m_hashCode;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
