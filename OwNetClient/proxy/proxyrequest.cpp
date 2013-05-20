#include "proxyrequest.h"
#include "response.h"

#include "messagehelper.h"
#include "jsondocument.h"
#include "requestreader.h"
#include "cachehelper.h"
#include "proxyconnection.h"

#include <QNetworkRequest>
#include <QStringList>
#include <QRegularExpression>
#include <QDebug>
#include <QUrlQuery>

ProxyRequest::ProxyRequest(RequestReader *requestReader, QObject *parent)
    : QObject(parent),
      m_isApiRequest(false),
      m_requestType(UNKNOWN),
      m_requestReader(requestReader)
{
    m_url = requestReader->url();
    m_requestBody = requestReader->requestBody();
    m_peerAddress = requestReader->peerAddress();
    m_peerPort = requestReader->peerPort();

    if (!requestReader->temporaryFile().fileName().isEmpty()) {
        m_multipartContentTempFilePath = requestReader->temporaryFile().fileName();
        MessageHelper::debug(m_multipartContentTempFilePath);
    }

    analyzeUrl();
    analyzeRequestHeaders(requestReader);
    analyzeRequestType(requestReader);
}

ProxyRequest::ProxyRequest(IRequest::RequestType requestType, QString url, QVariantMap requestHeaders, QObject *parent)
    : QObject(parent),
      m_isApiRequest(false),
      m_requestType(requestType),
      m_url(url),
      m_requestHeaders(requestHeaders),
      m_requestReader(NULL)
{
    analyzeUrl();
}

QString ProxyRequest::url() const
{
    return m_url;
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
    JsonDocument json = JsonDocument::fromJson(requestBody(), &err);

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

    QString body(requestBody());
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

QString ProxyRequest::parameterValue(const QString &key) const
{
    return QUrlQuery(QUrl(url()).query()).queryItemValue(key);
}

bool ProxyRequest::hasParameter(const QString &key) const
{
    return QUrlQuery(QUrl(url()).query()).hasQueryItem(key);
}

QMap<QString, QString> ProxyRequest::paramaters() const
{
    QList<QPair<QString, QString> > params = QUrlQuery(QUrl(url()).query()).queryItems();
    QMap<QString, QString> paramsMap;
    foreach (auto pair, params) {
        paramsMap.insert(pair.first, pair.second);
    }
    return paramsMap;
}

uint ProxyRequest::hashCode() const
{
    return CacheHelper::cacheId(url());
}

QString ProxyRequest::relativeUrl() const
{
    QString path = QUrl(url()).path(QUrl::FullyEncoded);
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

bool ProxyRequest::isRefreshRequest() const
{
    QString cacheControl = m_requestHeaders.value("Cache-Control").toString();
    QString pragma = m_requestHeaders.value("Pragma").toString();

    return cacheControl=="max-age=0" || cacheControl=="no-cache" || pragma=="No-cache";
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

IProxyConnection *ProxyRequest::proxyConnection()
{
    return new ProxyConnection(this);
}

/**
 * @brief Extracts the extension from the url.
 * @return Url extension
 */
QString ProxyRequest::urlExtension() const
{
    QStringList parts = url().split(".");
    if (parts.count() > 1)
        return parts.last();
    return "";
}

/**
 * @brief Analyzes the url and parses out the domain, subdomain and module, action and id for local requests.
 */
void ProxyRequest::analyzeUrl()
{
    QStringList domainSplit = QString(QUrl(url()).host(QUrl::FullyEncoded)).split(".");
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
            }
        }
    }
}

void ProxyRequest::analyzeRequestHeaders(RequestReader *request)
{
    QMapIterator<QByteArray,QByteArray> i(request->headerMap());
    while (i.hasNext()) {
        i.next();
        QString key(i.key());
        QString value(i.value());
        m_requestHeaders.insert(key, value);
    }
}

void ProxyRequest::analyzeRequestType(RequestReader *request)
{
    QString requestMethod = QString(request->method()).toLower();
    if (requestMethod == "get")
        m_requestType = GET;
    else if (requestMethod == "post")
        m_requestType = POST;
    else if (requestMethod == "put")
        m_requestType = PUT;
    else if (requestMethod == "delete")
        m_requestType = DELETE;
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
