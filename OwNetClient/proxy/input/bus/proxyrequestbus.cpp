#include "proxyrequestbus.h"

#include "proxyrequest.h"
#include "qjson/serializer.h"

#include <QBuffer>
#include <QVariantList>

ProxyRequestBus::ProxyRequestBus(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_request(request)
{
    m_httpStatusCode = QString::number(200);
    m_httpStatusDescription = "OK";

    addHeader("Content-type", m_request->requestContentType());
}

void ProxyRequestBus::readRequest()
{
    QVariantList people;

    QVariantMap bob;
    bob.insert("Name", "Bob");
    bob.insert("Phonenumber", 123);

    QVariantMap alice;
    alice.insert("Name", "Alice");
    alice.insert("Phonenumber", 321);

    people << bob << alice;

    QJson::Serializer serializer;
    QByteArray *json = new QByteArray(serializer.serialize(people));

    QBuffer *buffer = new QBuffer(json);
    buffer->open(QIODevice::ReadOnly);
    emit readyRead(buffer);

    emit finished();
}
