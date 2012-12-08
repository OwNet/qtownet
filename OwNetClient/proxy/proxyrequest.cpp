#include "proxyrequest.h"
#include "response.h"

#include "messagehelper.h"
#include "jsondocument.h"

#include <QNetworkRequest>
#include <QStringList>
#include <QTcpSocket>
#include <QRegularExpression>
#include <QDebug>

ProxyRequest::ProxyRequest(QTcpSocket *socket, QObject *parent)
    : QObject(parent),
      // m_id(-1),
      m_hashCode(-1),
      m_isApiRequest(false),
      m_socket(socket)
{
}

/**
 * @brief Reads request headers from socket.
 * @return Returns true if successful
 */
bool ProxyRequest::readFromSocket()
{
    for (int i = 0; m_socket->isOpen() && m_socket->canReadLine(); ++i) {
        QString readLine(m_socket->readLine());

        if (i == 0) {
            QStringList tuple = readLine.split(QRegularExpression("[ \r\n][ \r\n]*"));
            if (tuple.count() > 1) {
                m_requestMethod = tuple.first().toLower();
                m_qUrl = QUrl::fromEncoded(tuple.at(1).toUtf8());
                m_qUrlQuery = QUrlQuery(m_qUrl);
            } else {
                return false;
            }
        } else {
            if (readLine == "\r\n") {
                m_requestBody = m_socket->readAll();
            } else {
                QStringList tokens = readLine.split(":");
                if (tokens.count() < 2)
                    continue;
                QString key = tokens.first();
                QString value = tokens.at(1);
                value.remove(QRegularExpression("[\r\n][\r\n]*"));

                if (!key.toLower().contains("accept-encoding") || !value.contains("gzip")) {
                    m_requestHeaders.insert(key, value);
                } else {
                    m_requestHeaders.insert("Accept-encoding", "*");
                }
            }
        }
    }

    analyzeUrl();

    return true;
}

/**
 * @brief Get the HTTP request type.
 * @return HTTP request type
 */
ProxyRequest::RequestType ProxyRequest::requestType() const
{
    if (m_requestMethod == "get")
        return GET;
    else if (m_requestMethod == "post")
        return POST;
    else if (m_requestMethod == "put")
        return PUT;
    else if (m_requestMethod == "delete")
        return DELETE;
    return UNKNOWN;
}

/**
 * @brief Reads the body of the request as a JSON
 * @return Return the request body as QVariantMap
 */
QVariant ProxyRequest::postBodyFromJson(bool *ok) const
{
    QVariantMap result;
    if(requestType() != POST && requestType() != PUT)
        return result;

    QJsonParseError err;
    JsonDocument json = JsonDocument::fromJson(m_requestBody, &err);

    if (ok) {
        *ok = (err.error == QJsonParseError::NoError);

        if ( !(*ok) )
            return result;
    }

    return json.toVariant();
}

/**
 * @brief Parses the request body as received from HTML form.
 * @return Map of keys and values in the body
 */
QMap<QString, QString> ProxyRequest::postBodyFromForm() const
{
    QMap<QString, QString> result;
    if(requestType() != POST && requestType() != PUT)
        return result;

    QString body(m_requestBody);
    body.replace("+", " ");
    body.replace("%21", "!");
    body.replace("%22", "\"");
    body.replace("%23", "#");
    body.replace("%24", "$");
    body.replace("%25", "%");
    body.replace("%27", "'");
    body.replace("%28", "(");
    body.replace("%29", ")");
    body.replace("%2A", "*");
    body.replace("%2B", "+");
    body.replace("%2C", ",");
    body.replace("%2D", "-");
    body.replace("%2E", ".");
    body.replace("%2F", "/");
    body.replace("%3A", ":");
    body.replace("%3B", ";");
    body.replace("%3C", "<");
    body.replace("%3E", ">");
    body.replace("%3F", "?");
    body.replace("%40", "@");
    body.replace("%5B", "[");
    body.replace("%5D", "]");
    body.replace("%5F", "_");
    QStringList split = body.split("&");
    for (int i = 0; i < split.count(); i++){
        QStringList paramsKeyValue = split.at(i).split("=");
        QString key = paramsKeyValue.first();
        QString value = paramsKeyValue.last();
        value.replace("%26", "&");
        value.replace("%3D", "=");
        result.insert(key, value);
    }
    return result;
}

/**
 * @brief Get the content type from the url extension.
 * @return Content type of the request
 */
QString ProxyRequest::requestContentType(const QString &defaultContentType, const QString &extension) const
{
    QString ext = extension.isEmpty() ? urlExtension() : extension;
    if (ProxyRequest::m_contentTypes.contains(ext))
        return ProxyRequest::m_contentTypes.value(ext);
    return defaultContentType.isEmpty() ? "application/octet-stream" : defaultContentType;
}

QString ProxyRequest::relativeUrl() const
{
    QString path = m_qUrl.path(QUrl::FullyEncoded);
    if (path.startsWith('/'))
        path.remove(0, 1);
    return path;
}

/**
 * @brief Returns path to the requested static file.
 * @return Path to the requested static file
 */
QString ProxyRequest::staticResourcePath() const
{
    if (isStaticResourceRequest())
    {
        if (!subDomain().isEmpty()) {
            return QString ("static/%1/%2").arg(subDomain()).arg(relativeUrl());
        }
        return QString("static/%1").arg(relativeUrl());
    }
    return "";
}

bool ProxyRequest::isLocalRequest() const
{
    if (m_domain == "ownet")
        return true;
    return false;
}

/**
 * @brief Checks if the request is for a local static file.
 * @return True if requests a local proxy file.
 */
bool ProxyRequest::isStaticResourceRequest() const
{
    return isLocalRequest() && !isApiRequest();
}

IResponse *ProxyRequest::response()
{
    return new Response();
}

IResponse *ProxyRequest::response(const QVariant body, IResponse::Status status)
{
    return (new Response())->setBody(body)->setStatus(status);
}

IResponse *ProxyRequest::response(IResponse::Status status)
{
    return (new Response())->setStatus(status);
}

/**
 * @brief Extracts the extension from the url.
 * @return Url extension
 */
QString ProxyRequest::urlExtension() const
{
    QStringList parts = m_qUrl.path().split(".");
    if (parts.count() > 1)
        return parts.last();
    return "";
}

/**
 * @brief Analyzes the url and parses out the domain, subdomain and module, action and id for local requests.
 */
void ProxyRequest::analyzeUrl()
{
    m_hashCode = qHash(url());

    QStringList domainSplit = QString(m_qUrl.host(QUrl::FullyEncoded)).split(".");
    if (domainSplit.first() == "www")
        domainSplit.takeFirst();

    if (domainSplit.count() > 0) {
        m_domain = domainSplit.takeLast();
        m_subDomain = domainSplit.join(".");
    }

    if (isLocalRequest()) {
        QStringList split = relativeUrl().remove(QRegularExpression("^[/]")).split("/");

        if (split.first() == "api") {
            split.takeFirst();
            m_isApiRequest = true;

            if (split.count()) {
                m_service = split.takeFirst();

//                if (split.count()) {
//                    QString idOrAction = split.first();
//                    bool ok;
//                    int id = idOrAction.toInt(&ok);
//                    if (ok) {
//                        m_id = id;
//                        split.takeFirst();
//                    }
//                    if (split.count())
//                        m_action = split.join("/");
//                }
            }
        }
    }
}

/**
 * @brief The known content types based on the url extension.
 */
QMap<QString, QString> ProxyRequest::m_contentTypes = initContentTypes();
QMap<QString, QString> ProxyRequest::initContentTypes()
{
    QMap<QString, QString> map;
    map.insert("htm", "text/html");
    map.insert("html", "text/html");
    map.insert("css", "text/css");
    map.insert("csv", "text/csv");
    map.insert("js", "application/javascript");
    map.insert("json", "application/json");
    map.insert("xml", "text/xml");
    map.insert("avi", "video/x-msvideo");
    map.insert("bmp", "image/bmp");
    map.insert("png", "image/png");
    map.insert("gif", "image/gif");
    map.insert("jpg", "image/jpeg");
    map.insert("jpeg", "image/jpeg");
    return map;
}
