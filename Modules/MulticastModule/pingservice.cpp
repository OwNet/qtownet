#include "pingservice.h"

#include "pingserver.h"
#include "irouter.h"

PingService::PingService(PingServer *pingServer, QObject *parent) :
    QObject(parent),
    m_pingServer(pingServer)
{
}

void PingService::init(IRouter *router)
{
    router->addRoute("/available_clients")
            ->on(IRequest::POST, ROUTE(availableClients));
}

IResponse *PingService::availableClients(IRequest *request)
{
    bool ok = false;
    QVariantMap requestBody = request->postBodyFromJson(&ok).toMap();
    if (!ok)
        return request->response(IResponse::BAD_REQUEST);
    requestBody.insert("address", request->peerAddress());
    m_pingServer->updateClient(requestBody);

    return request->response(m_pingServer->availableClients());
}
