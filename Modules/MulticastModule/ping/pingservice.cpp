#include "pingservice.h"

#include "pingserver.h"
#include "irouter.h"

PingService::PingService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent)
{
    m_pingServer = new PingServer(proxyConnection, this);
}

void PingService::init(IRouter *router)
{
    router->addRoute("/available_clients")
            ->on(IRequest::POST, ROUTE(availableClients));
    router->addRoute("/hi")
            ->on(IRequest::GET, ROUTE(hi));
}

IResponse *PingService::availableClients(IRequest *request)
{
    bool ok = false;
    QVariantMap requestBody = request->postBodyFromJson(&ok).toMap();
    if (!ok)
        return request->response(IResponse::BAD_REQUEST);
    requestBody.insert("address", request->peerAddress());
    m_pingServer->updateClient(requestBody);

    return request->response(m_pingServer->availableClients(requestBody.value("called_ip").toString()));
}

IResponse *PingService::hi(IRequest *request)
{
    QVariantMap map;
    map.insert("Hi!", "Nice to meet you.");
    map.insert("Lots of love and kisses", "Team OwNet");
    return request->response(map);
}
