#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include "irequest.h"
#include "variantmap.h"

#include <QMap>
#include <QByteArray>
#include <QUrl>
#include <QUrlQuery>
#include <QStringList>

/**
 * @brief Represents all the information about the request received by proxy.
 */
class ProxyRequest : public QObject, public IRequest
{
    Q_OBJECT

public:
    ProxyRequest(QObject *parent = 0);

    QVariant postBodyFromJson(bool *ok = NULL) const;
    QMap<QString, QString> postBodyFromForm() const;
    QByteArray requestBody() const { return m_requestBody; }
    void setRequestBody(const QByteArray &requestBody) { m_requestBody = requestBody; }

    void addRequestHeader(const QString &key, const QString &value);
    QVariantMap requestHeaders() const { return m_requestHeaders; }

    IRequest::RequestType requestType() const { return m_requestType; }
    void setRequestType(IRequest::RequestType requestType) { m_requestType = requestType; }

    QUrl qUrl() const { return m_qUrl; }
    QString url() const { return m_qUrl.toEncoded(QUrl::None); }
    QString relativeUrl() const;
    void setUrl(const QString &url);

    QString requestContentType(const QString &defaultContentType = "", const QString &extension = "") const;

    QString parameterValue(const QString &key) const { return m_qUrlQuery.queryItemValue(key); }
    bool hasParameter(const QString& key) const { return m_qUrlQuery.hasQueryItem(key); }
    QStringList allParameterValues(const QString &key) const { return m_qUrlQuery.allQueryItemValues(key); }

    uint hashCode() const { return m_hashCode; }

    QString subDomain() const { return m_subDomain; }
    QString service() const { return isLocalRequest() ? m_service : QString(); }
    QString staticResourcePath() const;
    bool isLocalRequest() const;
    bool isStaticResourceRequest() const;
    bool isApiRequest() const { return m_isApiRequest; }

    IResponse* response();
    IResponse* response(const QVariant body, IResponse::Status status = IResponse::OK);
    IResponse* response(IResponse::Status status);

protected:
    void setUrl(const QUrl &url);

private:
    QString urlExtension() const;
    void analyzeUrl();
    static QMap<QString, QString> initContentTypes();

    uint m_hashCode;

    bool m_isApiRequest;

    QByteArray m_requestBody;
    static QMap<QString, QString> m_contentTypes;
    VariantMap m_requestHeaders;
    QUrl m_qUrl;
    QUrlQuery m_qUrlQuery;
    IRequest::RequestType m_requestType;
    QString m_domain;
    QString m_subDomain;
    QString m_service;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
