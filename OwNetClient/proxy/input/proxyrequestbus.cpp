#include "proxyrequestbus.h"

#include "proxyrequest.h"
#include "requestrouter.h"
#include "session.h"
#include "databaseupdate.h"
#include "settings.h"
#include "requestrouter.h"
#include "iresponse.h"
#include "jsondocument.h"
#include "listofstringpairs.h"

#include <QBuffer>
#include <QVariantList>
#include <QDebug>

ProxyRequestBus::ProxyRequestBus(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_request(request)
{
}

void ProxyRequestBus::readRequest()
{
    IResponse* response = RequestRouter::processRequest(m_request);
    QByteArray* byteResponse = processResponse(response);

    QBuffer *buffer = new QBuffer( byteResponse );
    buffer->open(QIODevice::ReadOnly);
    emit readyRead(buffer);
    emit finished();
}

QByteArray *ProxyRequestBus::processResponse(IResponse *response)
{
    QByteArray *result = NULL;

    if (response == NULL)
        return NULL;

    setHttpStatusCode(response->status());
    setHttpStatusDescription(response->statusMessage());

    auto headers = response->headers();
    QList<QString> keys = headers.keys();

    for (int i = 0; i < keys.count(); ++i)
        addHeader(keys[i], headers.value(keys[i]));

    QVariant body = response->body();

    if (body.isNull())
        return NULL;

    bool notContentType = (contentType()=="");

    switch (body.type()) {

    case QMetaType::QVariantList:
    case QMetaType::QVariantMap:
        result = new QByteArray(JsonDocument::fromVariant(body).toJson());
        if (notContentType)
            setContentType("text/json");
        break;

    case QMetaType::QByteArray:
        result = new QByteArray(body.toByteArray());
        if (notContentType)
            setContentType("text");
        break;

    case QMetaType::QJsonDocument:
        result = new QByteArray(JsonDocument( body.toJsonDocument() ).toJson());
        if (notContentType)
            setContentType("text/json");
        break;

    default:
        throw "Invalid data type";

    }

    return result;
}
