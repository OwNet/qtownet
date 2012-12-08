#ifndef REQUESTROUTER_H
#define REQUESTROUTER_H

#include <QObject>
#include <QVector>
#include "route.h"
#include "irouter.h"
#include "proxyconnection.h"

class IRestService;
class IService;
class IBus;
class IRequest;
class IResponse;

typedef std::function<IResponse* (IRequest *req)> Callback;

class RequestRouter : public QObject, public IRouter
{
    Q_OBJECT
public:
    RequestRouter(IService* service, QObject *parent = 0);
    RequestRouter(IRestService* service, QObject *parent = 0);

    IRoute* addRoute(QString url);
    void   setDefaultRoute(Callback);

    IService* serivce() { return m_service; }
    IRestService* restSerivce() { return m_restService; }

    IResponse *routeRequest(IRequest *req) const;
    static IResponse *processRequest(IRequest *req);

    static IService* getSerivce(const QString name);
    static IRestService* getRestSerivce(const QString name);

private:
    IService* m_service;
    IRestService* m_restService;
    QVector<Route*> m_routes;    

    bool m_hasDefaultRoute;
    Callback m_defaultRoute;

    static void addService(IService *service);
    static void addService(IRestService *service);
    static QMap<QString, RequestRouter*> *m_services;

    friend class ProxyConnection;
};

#endif // REQUESTROUTER_H
