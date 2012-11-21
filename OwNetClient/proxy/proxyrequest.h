#ifndef PROXYREQUEST_H
#define PROXYREQUEST_H

#include "listofstringpairs.h"
#include "irequest.h"

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
    ListOfStringPairs requestHeaders() const { return m_requestHeaders; }

    QUrl qUrl() const { return m_qUrl; }
    QString url() const { return m_qUrl.toEncoded(QUrl::None); }
    QString requestContentType(const QString &defaultContentType = "", const QString &extension = "") const;
    QString relativeUrl() const { return m_qUrl.encodedPath(); }
    QString action() const { return m_action; }
    QString module() const { return isLocalRequest() ? m_module : QString(); }
    QString subDomain() const { return m_subDomain; }

    int id() const { return m_id; }
    QString parameterValue(const QString &key) const { return m_qUrl.queryItemValue(key); }
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

    QString m_requestMethod;
    QString m_domain;
    QString m_subDomain;
    QString m_module;
    QString m_action;
    int m_id;
    QByteArray m_requestBody;
    bool m_isApiRequest;
    QUrl m_qUrl;

    QTcpSocket *m_socket;
    static QMap<QString, QString> m_contentTypes;
    ListOfStringPairs m_requestHeaders;
    int m_hashCode;

    friend class ProxyInitializer;
};

#endif // PROXYREQUEST_H
