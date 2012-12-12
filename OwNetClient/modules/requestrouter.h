#ifndef REQUESTROUTER_H
#define REQUESTROUTER_H

#include <QObject>
#include <QVector>
#include "route.h"
#include "irouter.h"
#include "proxyconnection.h"

class IRestService;
class IService;
class IRequest;
class IResponse;

class RequestRouter : public QObject, public IRouter
{
    Q_OBJECT
public:
    RequestRouter(IService* service, QObject *parent = 0);
    RequestRouter(IRestService* service, QObject *parent = 0);

    IRoute* addRoute(QString url);
    void setDefaultRoute(DefaultCallback);

    IService* service() { return m_service; }
    IRestService* restService() { return m_restService; }

    IResponse *routeRequest(IRequest *req) const;
    static IResponse *processRequest(IRequest *req);

    static IService* getService(const QString name);
    static IRestService* getRestService(const QString name);

private:
    IService* m_service;
    IRestService* m_restService;
    QVector<Route*> m_routes;    

    bool m_hasDefaultRoute;
    DefaultCallback m_defaultRoute;

    static void addService(IService *service);
    static void addService(IRestService *service);
    static QMap<QString, RequestRouter*> *m_services;

    friend class ProxyConnection;
};

#endif // REQUESTROUTER_H
