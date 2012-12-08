#include "requestrouter.h"

#include "irequest.h"
#include "ibus.h"
#include "iservice.h"
#include "irestservice.h"

QMap<QString, RequestRouter*> *RequestRouter::m_services = new QMap<QString, RequestRouter*>();

void RequestRouter::addService(IService* service)
{
    m_services->insert(service->name(), new RequestRouter(service) );
}

void RequestRouter::addService(IRestService *service)
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
    else {
        return req->response(IResponse::NOT_FOUND);
    }
}

IService *RequestRouter::getSerivce(const QString name)
{
    if (m_services->contains(name))
        return m_services->value(name)->serivce();
    else
        return NULL;
}

IRestService *RequestRouter::getRestSerivce(const QString name)
{
    if (m_services->contains(name))
        return m_services->value(name)->restSerivce();
    else
        return NULL;
}

RequestRouter::RequestRouter(IService *service, QObject *parent)
    : QObject(parent),
      m_service(service),
      m_restService(NULL),
      m_hasDefaultRoute(false)
{
    service->init(this);
}

RequestRouter::RequestRouter(IRestService *service, QObject *parent)
    : QObject(parent),
      m_service(service),
      m_restService(service),
      m_hasDefaultRoute(false)
{
    addRoute("/")
            ->on(IRequest::GET, ROUTE(m_restService->index))
            ->on(IRequest::POST, ROUTE(m_restService->create));

    addRoute("/:id")
            ->on(IRequest::GET, ROUTE(m_restService->show, INT(id) ))            
            ->on(IRequest::PUT, ROUTE(m_restService->edit, INT(id) ))
            ->on(IRequest::DELETE, ROUTE(m_restService->del, INT(id) ));

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

Route* RequestRouter::addRoute(QString url)
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
        m_routes.append(route);
    }

    return route;
}

void RequestRouter:: setDefaultRoute(Callback callback)
{

    m_defaultRoute = callback;
    m_hasDefaultRoute = true;
}
