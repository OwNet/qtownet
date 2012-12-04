#include "requestrouter.h"

#include "irequest.h"
#include "ibus.h"
#include "iservice.h"
#include "irestservice.h"
#include "qjson/serializer.h"

QMap<QString, IService*> *RequestRouter::m_services = new QMap<QString, IService*>();
QMap<QString, IRestService*> *RequestRouter::m_restServices = new QMap<QString, IRestService*>();

RequestRouter::RequestRouter(const QString &serviceName, QObject *parent)
    : QObject(parent),
      m_service(NULL),
      m_restService(NULL)
{
    if (m_services->contains(serviceName))
        m_service = m_services->value(serviceName);
    else if (m_restServices->contains(serviceName))
        m_restService = m_restServices->value(serviceName);
}

/**
 * @brief processRequest Virtual function for Processing request from proxy/
 *        every module derived from IModule has to implement this function in it's own way
 * @param bus
 * @param req
 * @return  response Bytes
 */
QByteArray *RequestRouter::processRequest(IBus *bus, IRequest *req) const
{
    QByteArray *response = NULL;
    if (m_service) {
        response = m_service->processRequest(bus, req);
    } else if (m_restService) {

        QVariant *json = processRestRequest(bus, req);

        if (json) {
            QJson::Serializer serializer;
            response = new QByteArray(serializer.serialize(*json));
            delete json;
        }
    }
    if (response)
        return response;

    QVariantMap status;
    status.insert("Status", "FAILED");

    QJson::Serializer serializer;
    return new QByteArray(serializer.serialize(status));
}

QVariant *RequestRouter::processRestRequest(IBus *bus, IRequest *req) const
{
    if (!m_restService)
        return NULL;

    //get right action, also perserve other actions
    QVariant *json = NULL;

    if (req->action() == "" ||
            req->action() == "index" ||
            req->action() == "show" ||
            req->action() == "edit" ||
            req->action() == "delete" ||
            req->action() == "create") {

        //case index
        if (req->id() == -1 && req->requestType() == IRequest::GET)
            json = m_restService->index(bus, req);

        //case show
        else if (req->requestType() == IRequest::GET)
            json = m_restService->show(bus, req);

        //case create
        else if (req->requestType() == IRequest::POST)
            json = m_restService->create(bus, req);

        //case edit
        else if (req->requestType() == IRequest::PUT)
            json = m_restService->edit(bus, req);

        //case delete
        else if (req->requestType() == IRequest::DELETE)
            json = m_restService->del(bus, req);

        //other actions
        else
            json = m_restService->processRequest(bus, req);
    }
    else
        json = m_restService->processRequest(bus, req);
    return json;
}

void RequestRouter::addRoute(IService *service)
{
    m_services->insert(service->name(), service);
}

void RequestRouter::addRoute(IRestService *service)
{
    m_restServices->insert(service->name(), service);
}
