#include "requestbus.h"

#include "proxyrequest.h"
#include "qjson/serializer.h"

#include <QBuffer>
#include <QVariantList>

RequestBus::RequestBus(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_request(request)
{
    m_responseHeaders.insert("Content-type", m_request->requestContentType());
}

void RequestBus::readRequest()
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
    QByteArray json = serializer.serialize(people);

    QBuffer buffer(&json);
    buffer.open(QIODevice::ReadOnly);
    emit readyRead(&buffer, this);

    emit finished();
}
