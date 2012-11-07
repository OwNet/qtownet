#include "proxyrequest.h"
#include "messagehelper.h"

#include <QNetworkRequest>
#include <QStringList>
#include <QTcpSocket>
#include <QRegExp>

ProxyRequest::ProxyRequest(QTcpSocket *socket, QObject *parent)
    : QObject(parent), m_socket(socket), m_hashCode(-1)
{
}

bool ProxyRequest::readFromSocket()
{
    for (int i = 0; m_socket->isOpen() && m_socket->canReadLine(); ++i) {
        QString readLine(m_socket->readLine());

        if (i == 0) {
            QStringList tuple = readLine.split(QRegExp("[ \r\n][ \r\n]*"));
            if (tuple.count() > 1) {
                m_requestMethod = tuple.first().toLower();
                m_url = tuple.at(1);
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

ProxyRequest::RequestType ProxyRequest::requestType()
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

const QString ProxyRequest::requestContentType()
{
    QString ext = urlExtension();
    if (ProxyRequest::m_contentTypes.contains(ext))
        return ProxyRequest::m_contentTypes.value(ext);
    return "application/octet-stream";
}

const QString ProxyRequest::urlExtension()
{
    QStringList parts = m_url.split("?").first().split(".");
    if (parts.count() > 1)
        return parts.last();
    return "";
}

void ProxyRequest::analyzeUrl()
{
    m_hashCode = qHash(m_url);

    QStringList httpSplit = m_url.split("//");
    httpSplit.takeFirst();
    QString url = httpSplit.join("//");
    QStringList split = url.split("/");
    QString fullDomain = split.takeFirst();
    QStringList domainSplit = fullDomain.split(".");
    if (domainSplit.first() == "www")
        domainSplit.takeFirst();

    if (domainSplit.count() > 0) {
        m_domain = domainSplit.takeLast();
        m_subDomain = domainSplit.join(".");
    }

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
    }

}

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
