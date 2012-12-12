#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include "irequest.h"
#include "variantmap.h"

#include <QMap>
#include <QByteArray>
#include <QUrl>
#include <QUrlQuery>
#include <QStringList>

class QTcpSocket;

/**
 * @brief Represents all the information about the request received by proxy.
 */
class ProxyRequest : public QObject, public IRequest
{
    Q_OBJECT

public:
    ProxyRequest(QTcpSocket *socket, QObject *parent = 0);

    QVariant postBodyFromJson(bool *ok = NULL) const;
    QMap<QString, QString> postBodyFromForm() const;

    bool readFromSocket();

    IRequest::RequestType requestType() const;
    QVariantMap requestHeaders() const { return m_requestHeaders; }

    QUrl qUrl() const { return m_qUrl; }
    QString url() const { return m_qUrl.toEncoded(QUrl::None); }
    QString requestContentType(const QString &defaultContentType = "", const QString &extension = "") const;

    QString subDomain() const { return m_subDomain; }
    QString service() const { return isLocalRequest() ? m_service : QString(); }
    QString relativeUrl() const;
//    QString action() const { return m_action; }



//    int id() const { return m_id; }
    QString parameterValue(const QString &key) const { return m_qUrlQuery.queryItemValue(key); }
    bool hasParameter(const QString& key) const { return m_qUrlQuery.hasQueryItem(key); }
    QStringList allParameterValues(const QString &key) const { return m_qUrlQuery.allQueryItemValues(key); }
    QByteArray requestBody() const { return m_requestBody; }
    QString staticResourcePath() const;

    int hashCode() const { return m_hashCode; }

    bool isLocalRequest() const;
    bool isStaticResourceRequest() const;
    bool isApiRequest() const { return m_isApiRequest; }

    QTcpSocket *socket() const { return m_socket; }

    IResponse* response();
    IResponse* response(const QVariant body, IResponse::Status status = IResponse::OK);
    IResponse* response(IResponse::Status status);

private:
    QString urlExtension() const;
    void analyzeUrl();
    static QMap<QString, QString> initContentTypes();

    // int m_id;
    int m_hashCode;

    bool m_isApiRequest;

    QByteArray m_requestBody;
    QTcpSocket *m_socket;
    static QMap<QString, QString> m_contentTypes;
    VariantMap m_requestHeaders;
    QUrl m_qUrl;
    QUrlQuery m_qUrlQuery;
    QString m_requestMethod;
    QString m_domain;
    QString m_subDomain;
    QString m_service;
    // QString m_action;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
