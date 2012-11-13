#include "proxyrequest.h"
#include "messagehelper.h"
#include "qjson/parser.h"

#include <QNetworkRequest>
#include <QStringList>
#include <QTcpSocket>
#include <QRegExp>

ProxyRequest::ProxyRequest(QTcpSocket *socket, QObject *parent)
    : QObject(parent), m_socket(socket), m_hashCode(-1), m_isApiRequest(false), m_id(-1)
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
            QStringList tuple = readLine.split(QRegExp("[ \r\n][ \r\n]*"));
            if (tuple.count() > 1) {
                m_requestMethod = tuple.first().toLower();
                m_qUrl = QUrl::fromEncoded(tuple.at(1).toUtf8());
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
                value.remove(QRegExp("[\r\n][\r\n]*"));

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

QVariantMap ProxyRequest::postBodyFromJson() const
{
    QVariantMap result;
    if(requestType() != POST && requestType() != PUT)
        return result;

    bool ok;
    QJson::Parser parser;
    result = parser.parse(m_requestBody, &ok).toMap();
    return result;
}

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

/**
 * @brief Returns path to the requested static file.
 * @return Path to the requested static file
 */
QString ProxyRequest::staticResourcePath() const
{
    if (isStaticResourceRequest())
        if (!subDomain().isEmpty())
            return QString ("static/%1/%2").arg(subDomain()).arg(relativeUrl());

    return QString("static/%1").arg(relativeUrl());

    return "";
}

/**
 * @brief Checks if the request is for a local static file.
 * @return True if requests a local proxy file.
 */
bool ProxyRequest::isStaticResourceRequest() const
{
    return isLocalRequest() && !isApiRequst();
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
<<<<<<< HEAD
    m_id = 0;

    m_hashCode = qHash(m_url);

    QStringList httpSplit = m_url.split("//");
    httpSplit.takeFirst();
    QString url = httpSplit.join("//");
    QStringList split = url.split("/");
    QString fullDomain = split.takeFirst();
    QStringList domainSplit = fullDomain.split(".");
=======
    m_hashCode = qHash(url());

    QStringList domainSplit = QString(m_qUrl.encodedHost()).split(".");
>>>>>>> master
    if (domainSplit.first() == "www")
        domainSplit.takeFirst();

    if (domainSplit.count() > 0) {
        m_domain = domainSplit.takeLast();
        m_subDomain = domainSplit.join(".");
    }

<<<<<<< HEAD
    if (split.count() > 0) {
       QStringList params = split.join("/").split("?");
       m_relativeUrl = params.takeFirst();
       split = m_relativeUrl.split("/");
       m_module = split.takeFirst();
       if (split.count() > 0){
           QString idOrAction = split.first();
           bool ok;
           int id = idOrAction.toInt(&ok);
           if(ok){

               m_id = id;
               split.takeFirst();
           }

           if (split.count())
               m_action = split.join("/");
       }
       if (params.count() > 0){
           params = params.join("?").split("&");
           for (int i = 0; i < params.count(); i++){
               QStringList paramsKeyValue = params.at(i).split("=");
               QString key = paramsKeyValue.first();
               QString value = paramsKeyValue.last();
               m_parameters.insert(key, value);
           }
       }
    } else {
        m_relativeUrl = "";
=======
    if (isLocalRequest()) {
        QStringList split = relativeUrl().remove(QRegExp("^[/]")).split("/");

        if (split.first() == "api") {
            split.takeFirst();
            m_isApiRequest = true;

            if (split.count()) {
                m_module = split.takeFirst();

                if (split.count()) {
                    QString idOrAction = split.first();
                    bool ok;
                    int id = idOrAction.toInt(&ok);
                    if (ok) {
                        m_id = id;
                        split.takeFirst();
                    }
                    if (split.count())
                        m_action = split.join("/");
                }
            }
        }
>>>>>>> master
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
