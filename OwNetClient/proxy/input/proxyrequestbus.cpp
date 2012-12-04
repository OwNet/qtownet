#include "proxyrequestbus.h"

#include "proxyrequest.h"
#include "qjson/serializer.h"
#include "requestrouter.h"
#include "session.h"
#include "databaseupdate.h"
#include "settings.h"
#include "requestrouter.h"

#include <QBuffer>
#include <QVariantList>
#include <QDebug>

ProxyRequestBus::ProxyRequestBus(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_request(request)
{
    m_httpStatusCode = QString::number(200);
    m_httpStatusDescription = "OK";

    addHeader("Content-type", m_request->requestContentType("application/json"));
}

void ProxyRequestBus::readRequest()
{
    // returning processed request
    RequestRouter router(m_request->module());
    QBuffer *buffer = new QBuffer(router.processRequest(this, m_request));
    buffer->open(QIODevice::ReadOnly);
    emit readyRead(buffer);
    emit finished();
}

void ProxyRequestBus::setContentType(const QString &value)
{
    ProxyInputObject::setContentType(value);
}

void ProxyRequestBus::setHttpStatus(int code, const QString &description)
{
    setHttpStatusCode(code);
    setHttpStatusDescription(description);
}
