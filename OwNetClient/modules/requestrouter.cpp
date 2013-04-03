#include "requestrouter.h"

#include "irequest.h"
#include "iservice.h"
#include "irestservice.h"
#include "iuidrestservice.h"

QMap<QString, RequestRouter*> *RequestRouter::m_services = new QMap<QString, RequestRouter*>();

void RequestRouter::addService(IService* service)
{
    m_services->insert(service->name(), new RequestRouter(service) );
}

void RequestRouter::addService(IRestService *service)
{
    m_services->insert(service->name(), new RequestRouter(service) );
}

void RequestRouter::addService(IUidRestService *service)
{
    m_services->insert(service->name(), new RequestRouter(service) );
}

IResponse* RequestRouter::processRequest(IRequest *req)
{
    RequestRouter* router;
    QString serviceName = req->service();

    if (m_services->contains(serviceName)) {
        router = m_services->value(serviceName);
        return router->routeRequest(req);
    }
	return req->response(IResponse::NOT_FOUND);
}

IService *RequestRouter::getService(const QString name)
{
    if (m_services->contains(name))
        return m_services->value(name)->service();
    else
        return NULL;
}

IRestService *RequestRouter::getRestService(const QString name)
{
    if (m_services->contains(name))
        return m_services->value(name)->restService();
    else
        return NULL;
}

RequestRouter::RequestRouter(IService *service, QObject *parent)
    : QObject(parent),
      m_service(service),
      m_restService(NULL),
      m_uidRestService(NULL),
      m_hasDefaultRoute(false)
{
    service->init(this);
}

RequestRouter::RequestRouter(IRestService *service, QObject *parent)
    : QObject(parent),
      m_service(service),
      m_restService(service),
      m_uidRestService(NULL),
      m_hasDefaultRoute(false)
{
    addRoute("/")
            ->on(IRequest::GET, ROUTE(m_restService->index))
            ->on(IRequest::POST, ROUTE(m_restService->create));

    addRoute("/:id")
            ->on(IRequest::GET, ROUTE(m_restService->show, UINT(id) ))
            ->on(IRequest::PUT, ROUTE(m_restService->edit, UINT(id) ))
            ->on(IRequest::DELETE, ROUTE(m_restService->del, UINT(id) ));

    service->init(this);
}

RequestRouter::RequestRouter(IUidRestService *service, QObject *parent)
    : QObject(parent),
      m_service(service),
      m_restService(NULL),
      m_uidRestService(service),
      m_hasDefaultRoute(false)
{
    addRoute("/")
            ->on(IRequest::GET, ROUTE(m_uidRestService->index))
            ->on(IRequest::POST, ROUTE(m_uidRestService->create));

    addRoute("/:id")
            ->on(IRequest::GET, ROUTE(m_uidRestService->show, STR(id) ))
            ->on(IRequest::PUT, ROUTE(m_uidRestService->edit, STR(id) ))
            ->on(IRequest::DELETE, ROUTE(m_uidRestService->del, STR(id) ));

    service->init(this);
}

IResponse* RequestRouter::routeRequest(IRequest *req) const
{
    int size = m_routes.size();
    IRequest::RequestType method = req->requestType();

    QString url = req->relativeUrl();
    url.remove(0, 4+m_service->name().length()); // "/api/"+name

    if (!url.startsWith("/"))
        url = "/"+url;

    for (int i=0; i<size; i++) {
        Route* route =  m_routes[i];

        Route::Match match = route->regexp()->match(url);

        if (  match.hasMatch() ) {

            if ( route->callbacks()->contains(method) )
                return route->callbacks()->value(method)(req, match);
            else {
                req->response(IResponse::METHOD_NOT_ALLOWED);
            }
        }
    }

    if (m_hasDefaultRoute)
        return m_defaultRoute(req);

    return req->response(IResponse::NOT_FOUND);
}

IRoute* RequestRouter::addRoute(QString url)
{
    Route* route = NULL;

    int size = m_routes.size();
    for (int i=0; i<size; i++) {
        if (m_routes[i]->url()==url) {
            route = m_routes[i];
            break;
        }
    }

    if (route == NULL) {
        route = new Route(url);
        m_routes.prepend(route);
    }

    return route;
}

void RequestRouter::setDefaultRoute(DefaultCallback callback)
{
    m_defaultRoute = callback;
    m_hasDefaultRoute = true;
}
