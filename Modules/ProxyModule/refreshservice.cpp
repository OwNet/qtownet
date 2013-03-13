#include "refreshservice.h"

#include "irequest.h"
#include "irouter.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "refreshsession.h"

RefreshService::RefreshService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_refreshSession = new RefreshSession(m_proxyConnection, this);
}

void RefreshService::init(IRouter *router)
{
    router->addRoute("/start")
            ->on(IRequest::GET, ROUTE(start));
    router->addRoute("/stop")
            ->on(IRequest::GET, ROUTE(stop));
}

IResponse *RefreshService::start(IRequest *request)
{
    m_refreshSession->start();
    return request->response();
}

IResponse *RefreshService::stop(IRequest *request)
{
    m_refreshSession->stop();
    return request->response();
}
