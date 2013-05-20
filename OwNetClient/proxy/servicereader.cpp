#include "servicereader.h"

#include "iresponse.h"
#include "requestrouter.h"
#include "sockethandler.h"
#include "proxyrequest.h"
#include "jsondocument.h"

ServiceReader::ServiceReader(SocketHandler *socketHandler, ProxyRequest *request, QObject *parent) :
    QObject(parent),
    m_socketHandler(socketHandler),
    m_request(request)
{
}

void ServiceReader::read()
{
    IResponse* response = RequestRouter::processRequest(m_request);

    if (response) {
        m_socketHandler->writeStatusCodeAndDescription(response->status(), response->statusMessage().toUtf8());
        QMap<QString, QString> headers = response->headers();
        QVariantMap responseHeaders;
        bool noContentType = true;
        foreach (QString key, headers.keys()) {
            responseHeaders.insert(key, headers.value(key));
            if (!noContentType && key.compare("Content-type", Qt::CaseInsensitive) == 0)
                noContentType = false;
        }

        QVariant body = response->body();

        if (!body.isNull()) {
            QByteArray bytes;

            switch (body.type()) {

            case QMetaType::QVariantList:
            case QMetaType::QVariantMap:
                bytes = JsonDocument::fromVariant(body).toJson();
                if (noContentType)
                    responseHeaders.insert("Content-type", "application/json");
                break;

            case QMetaType::QByteArray:
                bytes = body.toByteArray();
                if (noContentType)
                    responseHeaders.insert("Content-type", "application/octet-stream");
                break;

            case QMetaType::QJsonDocument:
                bytes = JsonDocument( body.toJsonDocument() ).toJson();
                if (noContentType)
                    responseHeaders.insert("Content-type", "application/json");
                break;
            }
            responseHeaders.insert("Content-length", QString::number(bytes.length()));
            m_socketHandler->writeHeaders(responseHeaders);
            m_socketHandler->write(bytes);
            return;
        }
    }

    m_socketHandler->writeStatusCodeAndDescription(500, "Internal Server Error");
    QVariantMap responseHeaders;
    responseHeaders.insert("Content-length", 0);
    m_socketHandler->writeHeaders(responseHeaders);
    m_socketHandler->write("");
}
