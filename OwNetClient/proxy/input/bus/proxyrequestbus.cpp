#include "proxyrequestbus.h"

#include "proxyrequest.h"
#include "qjson/serializer.h"

#include <QBuffer>
#include <QVariantList>

QMap<QString, IModule*> *ProxyRequestBus::m_routes = new QMap<QString, IModule*>();

ProxyRequestBus::ProxyRequestBus(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_request(request)
{
    m_httpStatusCode = QString::number(200);
    m_httpStatusDescription = "OK";

    addHeader("Content-type", m_request->requestContentType());
}

void ProxyRequestBus::setHttpStatus( int code, QString description )
{
    m_httpStatusCode = QString::number(code);
    m_httpStatusDescription = description;
}

void ProxyRequestBus::readRequest()
{
    // checks if module exists
    if( m_routes->contains(m_request->module())) {
        // returning processed request
        QBuffer *buffer = new QBuffer(m_routes->value(m_request->module())->processRequest(this, m_request));
        buffer->open(QIODevice::ReadOnly);
        emit readyRead(buffer);
    }
    else {
        // ak chyba modifikovat po dohodnuti s matusom
        QVariantMap status;
        status.insert("Status", "FAILED");

        QJson::Serializer serializer;
        QByteArray *json = new QByteArray(serializer.serialize(status));

        QBuffer *buffer = new QBuffer(json);
        buffer->open(QIODevice::ReadOnly);
        emit readyRead(buffer);
    }
    emit finished();
}

/**
 * @brief ProxyRequestBus::callModule Function for modules,
 * they call it when they need to communicate with other modules
 * @param req
 * @return processedRequest from module in byte array
 */
QByteArray* ProxyRequestBus::callModule( ProxyRequest *req)
{
    // need to find only first part of url (module url)

    return m_routes->value(req->module())->processRequest(this,req);
}

void ProxyRequestBus::registerModule(IModule *newModule, QString url)
{
    m_routes->insert(url, newModule);
}
