#include "requestrouter.h"

#include "irequest.h"
#include "ibus.h"
#include "iservice.h"
#include "irestservice.h"
#include "qjson/serializer.h"

RequestRouter::RequestRouter(IService *iService, QObject *parent)
    : QObject(parent),
      m_iService(iService),
      m_iRestService(NULL)
{
}

RequestRouter::RequestRouter(IRestService *iRestService, QObject *parent)
    : QObject(parent),
      m_iService(NULL),
      m_iRestService(iRestService)
{
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
    if (m_iService) {
        response = m_iService->processRequest(bus, req);
    } else if (m_iRestService) {
        QVariant *json = processRestRequest(bus, req);
        if (json) {
            QJson::Serializer serializer;
            response = new QByteArray(serializer.serialize(*json));
            delete json;
        }
    }
    if (response)
        return response;
    return new QByteArray();
}

QVariant *RequestRouter::processRestRequest(IBus *bus, IRequest *req) const
{
    if (!m_iRestService)
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
            json = m_iRestService->index(bus, req);

        //case show
        else if (req->requestType() == IRequest::GET)
            json = m_iRestService->show(bus, req);

        //case create
        else if (req->requestType() == IRequest::POST)
            json = m_iRestService->create(bus, req);

        //case edit
        else if (req->requestType() == IRequest::PUT)
            json = m_iRestService->edit(bus, req);

        //case delete
        else if (req->requestType() == IRequest::DELETE)
            json = m_iRestService->del(bus, req);

        //other actions
        else
            json = m_iRestService->processRequest(bus, req);
    }
    return json;
}

QString RequestRouter::moduleName() const
{
    if (m_iService)
        return m_iService->name();
    if (m_iRestService)
        return m_iRestService->name();
    return QString();
}
