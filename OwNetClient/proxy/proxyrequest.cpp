#include "proxyrequest.h"
#include "messagehelper.h"

#include <QNetworkRequest>
#include <QStringList>
#include <QTcpSocket>
#include <QRegExp>

ProxyRequest::ProxyRequest(QTcpSocket *socket)
    : m_socket(socket)
{}

bool ProxyRequest::readFromSocket()
{
    for (int i = 0; m_socket->isOpen() && m_socket->canReadLine(); ++i) {
        QString readLine(m_socket->readLine());

        if (i == 0) {
            QStringList tuple = readLine.split(QRegExp("[ \r\n][ \r\n]*"));
            if (tuple.count() > 1) {
                m_requestMethod = tuple.first().toLower();
                m_networkRequest.setUrl(QUrl(tuple.at(1)));
            } else {
                return false;
            }
        } else {
            QStringList tokens = readLine.split(":");
            if (tokens.count() < 2)
                continue;
            QString key = tokens.first();
            QString value = tokens.at(1);
            value.remove(QRegExp("[\r\n][\r\n]*"));

            m_networkRequest.setRawHeader(key.toLatin1(), value.toLatin1());
        }
    }
    m_networkRequest.setRawHeader("X-Proxied-By", "OwNet");
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

const QString ProxyRequest::relativeUrl()
{
    QStringList split = m_networkRequest.url().toString().split("//").last().split("/");
    if (split.count() > 1) {
        split.takeFirst();
        return split.join("/");
    }
    return "";
}

const QString ProxyRequest::url()
{
    return m_networkRequest.url().toString();
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
    QStringList parts = url().split("?").first().split(".");
    if (parts.count() > 1)
        return parts.last();
    return "";
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
