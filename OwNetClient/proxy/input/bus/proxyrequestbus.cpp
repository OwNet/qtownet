#include "proxyrequestbus.h"

#include "proxyrequest.h"
#include "qjson/serializer.h"

#include <QBuffer>
#include <QVariantList>

QMap<QString, IModule*> *ProxyRequestBus::m_routes = new QMap<QString, IModule*>();

ProxyRequestBus::ProxyRequestBus(ProxyRequest *request, QObject *parent)
    : ProxyInputObject(request, parent), m_request(request)
{
    addHeader("Content-type", m_request->requestContentType());
}

void ProxyRequestBus::readRequest()
{
    // json howTo
   /* QVariantList people;

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
    */

    // checks if module exists
    if( m_routes->contains(m_request->subDomain())){

        // returning processed request
          QBuffer buffer( m_routes->value(m_request->subDomain())->processRequest(this,m_request));
          buffer.open(QIODevice::ReadOnly);
          emit readyRead(&buffer, this);

          emit finished();
    }
    else{
        // ak chyba modifikovat po dohodnuti s matusom
        QVariantMap status;
        status.insert("Status", "FAILED");

        QJson::Serializer serializer;
        QByteArray json = serializer.serialize(status);

        QBuffer buffer2(&json);
        buffer2.open(QIODevice::ReadOnly);
        emit readyRead(&buffer2, this);

        emit finished();
    }



}

/**
 * @brief ProxyRequestBus::callModule Function for modules,
 *                         they call it when they need to communicate with other modules
 * @param req
 * @return processedRequest from module in byte array
 */
QByteArray* ProxyRequestBus::callModule( ProxyRequest *req)
{

    // need to find only first part of url (module url)


    return m_routes->value(req->subDomain())->processRequest(this,req);
}

void ProxyRequestBus::registerModule(IModule *newModule, QString url)
{
    m_routes->insert(url, newModule);

}
