#include "requestrouter.h"

#include "irequest.h"
#include "ibus.h"
#include "imodule.h"
#include "irestmodule.h"
#include "qjson/serializer.h"

RequestRouter::RequestRouter(IModule *iModule, QObject *parent)
    : QObject(parent),
      m_iModule(iModule),
      m_iRestModule(NULL)
{
}

RequestRouter::RequestRouter(IRestModule *iRestModule, QObject *parent)
    : QObject(parent),
      m_iModule(NULL),
      m_iRestModule(iRestModule)
{
}

/**
 * @brief processRequest Virtual function for Processing request from proxy/
 *        every module derived from IModule has to implement this function in it's own way
 * @param bus
 * @param req
 * @return  response Bytes
 */
QByteArray *RequestRouter::processRequest(IBus *bus, IRequest *req)
{
    QByteArray *response = NULL;
    if (m_iModule) {
        response = m_iModule->processRequest(bus, req);
    } else if (m_iRestModule) {
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
                json = m_iRestModule->index(bus, req);

            //case show
            else if (req->requestType() == IRequest::GET)
                json = m_iRestModule->show(bus, req);

            //case create
            else if (req->requestType() == IRequest::POST)
                json = m_iRestModule->create(bus, req);

            //case edit
            else if (req->requestType() == IRequest::PUT)
                json = m_iRestModule->edit(bus, req);

            //case delete
            else if (req->requestType() == IRequest::DELETE)
                json = m_iRestModule->del(bus, req);

            //other actions
            else
                json = m_iRestModule->processRequest(bus, req);
        }
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

QString RequestRouter::moduleName() const
{
    if (m_iModule)
        return m_iModule->name();
    if (m_iRestModule)
        return m_iRestModule->name();
    return QString();
}
