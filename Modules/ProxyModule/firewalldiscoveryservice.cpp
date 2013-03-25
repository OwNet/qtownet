#include "firewalldiscoveryservice.h"

#include "irouter.h"
#include "firewalldiscoverymanager.h"

FirewallDiscoveryService::FirewallDiscoveryService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void FirewallDiscoveryService::init(IRouter *router)
{
    router->addRoute("/ping_me")
            ->on(IRequest::GET, ROUTE(pingMe));
    router->addRoute("/ping")
            ->on(IRequest::GET, ROUTE(ping));
}

IResponse *FirewallDiscoveryService::pingMe(IRequest *request)
{
    FirewallDiscoveryManager *manager = new FirewallDiscoveryManager(m_proxyConnection);
    manager->ping(request->parameterValue("my_id"));
    return request->response(IResponse::OK);
}

IResponse *FirewallDiscoveryService::ping(IRequest *request)
{
    FirewallDiscoveryManager::setPingedBack(true);
    return request->response(IResponse::OK);
}
