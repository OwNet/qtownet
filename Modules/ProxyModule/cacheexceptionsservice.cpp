#include "cacheexceptionsservice.h"

#include "irequest.h"
#include "irouter.h"
#include "cacheexceptionsmanager.h"

CacheExceptionsService::CacheExceptionsService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void CacheExceptionsService::init(IRouter *router)
{
    router->addRoute("/add")
            ->on(IRequest::POST, ROUTE(add));
    router->addRoute("/remove")
            ->on(IRequest::POST, ROUTE(remove));
    router->addRoute("/check")
            ->on(IRequest::POST, ROUTE(check));
}

IResponse *CacheExceptionsService::add(IRequest *request)
{
    CacheExceptionsManager manager(m_proxyConnection);
    manager.addException(request->postBodyFromJson().toMap().value("url").toString());
    return request->response();
}

IResponse *CacheExceptionsService::remove(IRequest *request)
{
    CacheExceptionsManager manager(m_proxyConnection);
    manager.removeException(request->postBodyFromJson().toMap().value("url").toString());
    return request->response();
}

IResponse *CacheExceptionsService::check(IRequest *request)
{
    CacheExceptionsManager manager(m_proxyConnection);
    QVariantMap response;
    response.insert("is_exception", manager.isException(request->postBodyFromJson().toMap().value("url").toString()));
    return request->response(response);
}
