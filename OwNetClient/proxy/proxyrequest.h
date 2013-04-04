#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include "irequest.h"
#include "variantmap.h"

#include <QMap>
#include <QByteArray>
#include <QStringList>

class HttpRequest;

/**
 * @brief Represents all the information about the request received by proxy.
 */
class ProxyRequest : public QObject, public IRequest
{
    Q_OBJECT

public:
    enum {
        Port = 8081
    };

    ProxyRequest(HttpRequest *request, QObject *parent = 0);
    ProxyRequest(IRequest::RequestType requestType, QString url, QVariantMap requestHeaders, QObject *parent = 0);

    QVariant postBodyFromJson(bool *ok = NULL) const;
    QMap<QString, QString> postBodyFromForm() const;
    QByteArray requestBody() const { return m_requestBody; }
    QVariantMap requestHeaders() const { return m_requestHeaders; }
    IRequest::RequestType requestType() const { return m_requestType; }
    QString requestContentType(const QString &defaultContentType = "", const QString &extension = "") const;

    QString url() const;
    QString relativeUrl() const;
    void setUrl(const QString &url);

    QString parameterValue(const QString &key) const;
    bool hasParameter(const QString& key) const;
    QMap<QString, QString> paramaters() const;

    uint hashCode() const;

    QString subDomain() const { return m_subDomain; }
    QString service() const { return isLocalRequest() ? m_service : QString(); }
    QString staticResourcePath() const;
    bool isLocalRequest() const;
    bool isStaticResourceRequest() const;
    bool isApiRequest() const { return m_isApiRequest; }
    bool isRefreshRequest() const;

    IResponse* response();
    IResponse* response(const QVariant body, IResponse::Status status = IResponse::OK);
    IResponse* response(IResponse::Status status);

    QString peerAddress() const { return m_peerAddress; }
    quint16 peerPort() const { return m_peerPort; }

protected:
    void setUrl(const QUrl &url);

private:
    QString urlExtension() const;
    void analyzeUrl();
    void analyzeRequestHeaders(HttpRequest *request);
    void analyzeRequestType(HttpRequest *request);
    static QMap<QString, QString> initContentTypes();

    bool m_isApiRequest;

    static QMap<QString, QString> m_contentTypes;
    QString m_url;
    QString m_domain;
    QString m_subDomain;
    QString m_service;
    QByteArray m_requestBody;
    QVariantMap m_requestHeaders;
    IRequest::RequestType m_requestType;
    QString m_peerAddress;
    quint16 m_peerPort;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
