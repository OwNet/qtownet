#include "appservice.h"

#include "iproxyconnection.h"
#include "irouter.h"

AppService::AppService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void AppService::init(IRouter *router)
{
    router->addRoute("/quit")
            ->on(IRequest::GET, ROUTE(quit));
}

IResponse *AppService::quit(IRequest *request)
{
    m_proxyConnection->quit();
    return request->response();
}
