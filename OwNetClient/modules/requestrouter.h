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

typedef std::function<QVariant* (IBus *bus, IRequest *req)> Callback;

class RequestRouter : public QObject, public IRouter
{
    Q_OBJECT
public:
    RequestRouter(IService* service, QObject *parent = 0);
    RequestRouter(IRestService* service, QObject *parent = 0);

    Route* addRoute(QString url);
    void   setDefaultRoute(Callback);

    QVariant *routeRequest(IBus *bus, IRequest *req) const;
    static QVariant *processRequest(IBus *bus, IRequest *req);

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
