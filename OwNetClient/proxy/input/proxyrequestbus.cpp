#include "proxyrequestbus.h"

#include "proxyrequest.h"
#include "requestrouter.h"
#include "session.h"
#include "databaseupdate.h"
#include "settings.h"
#include "requestrouter.h"
#include "jsondocument.h"

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
    QVariant* response = RequestRouter::processRequest(this, m_request);
    QByteArray* byteResponse = processResponse(response);

    QBuffer *buffer = new QBuffer( byteResponse );
    buffer->open(QIODevice::ReadOnly);
    emit readyRead(buffer);
    emit finished();
}

QByteArray *ProxyRequestBus::processResponse(const QVariant *response)
{        
    QByteArray *result = NULL;    

    if (response == NULL)
        return NULL;

    switch (response->type()) {

    case QMetaType::QVariantList:
    case QMetaType::QVariantMap:
        result = new QByteArray(JsonDocument::fromVariant(*response).toJson());
        break;

    case QMetaType::QByteArray:
        result = new QByteArray(response->toByteArray());
        break;

    case QMetaType::QJsonDocument:
        result = new QByteArray(JsonDocument( response->toJsonDocument() ).toJson());
        break;

    default:
        throw "Invalid data type";

    }

    return result;
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
