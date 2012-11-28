#include "proxyinputobject.h"

#include "messagehelper.h"

ProxyInputObject::ProxyInputObject(ProxyRequest *request, QObject *parent)
    : QObject(parent), m_request(request), m_downloadStarted(false)
{
}

void ProxyInputObject::startDownload()
{
    if (!m_downloadStarted) {
        m_downloadStarted = true;
        readRequest();
    }
}

void ProxyInputObject::setHttpStatusCode(int code)
{
    m_httpStatusCode = QString::number(code);
}

void ProxyInputObject::setHttpStatusDescription(const QString &description)
{
    m_httpStatusDescription = description;
}

QString ProxyInputObject::contentType() const
{
    foreach (QString key, m_responseHeaders.keys()) {
        if (key.toLower().contains("content-type")) {
            return m_responseHeaders.value(key).toString();
        }
    }
    return "";
}

void ProxyInputObject::setContentType(const QString &value)
{
    m_responseHeaders.insertCaseInsensitive("Content-type", value);
}

void ProxyInputObject::addHeader(const QString &key, const QString &value)
{
    m_responseHeaders.insert(key, value);
}
