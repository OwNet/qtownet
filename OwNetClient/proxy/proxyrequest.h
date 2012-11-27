#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include "irequest.h"
#include "variantmap.h"

#include <QNetworkRequest>
#include <QMap>
#include <QByteArray>
#include <QUrl>
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

    QVariantMap postBodyFromJson() const;
    QMap<QString, QString> postBodyFromForm() const;

    bool readFromSocket();

    IRequest::RequestType requestType() const;
    QVariantMap requestHeaders() const { return m_requestHeaders; }

    QUrl qUrl() const { return m_qUrl; }
    QString url() const { return m_qUrl.toEncoded(QUrl::None); }
    QString requestContentType(const QString &defaultContentType = "", const QString &extension = "") const;
    QString relativeUrl() const { return m_qUrl.encodedPath(); }
    QString action() const { return m_action; }
    QString module() const { return isLocalRequest() ? m_module : QString(); }
    QString subDomain() const { return m_subDomain; }

    int id() const { return m_id; }
    QString parameterValue(const QString &key) const { return m_qUrl.queryItemValue(key); }
    bool hasParameter(const QString& key) const { return m_qUrl.hasQueryItem(key); }
    QStringList allParameterValues(const QString &key) const { return m_qUrl.allQueryItemValues(key); }
    QByteArray requestBody() const { return m_requestBody; }
    QString staticResourcePath() const;

    int hashCode() const { return m_hashCode; }

    bool isLocalRequest() const { return m_domain == "ownet"; }
    bool isStaticResourceRequest() const;
    bool isApiRequest() const { return m_isApiRequest; }

    QTcpSocket *socket() const { return m_socket; }

private:
    QString urlExtension() const;
    void analyzeUrl();
    static QMap<QString, QString> initContentTypes();

    int m_id;
    int m_hashCode;

    bool m_isApiRequest;

    QByteArray m_requestBody;
    QTcpSocket *m_socket;
    static QMap<QString, QString> m_contentTypes;
    VariantMap m_requestHeaders;
    QUrl m_qUrl;
    QString m_requestMethod;
    QString m_domain;
    QString m_subDomain;
    QString m_module;
    QString m_action;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
