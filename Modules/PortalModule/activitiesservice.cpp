#include "activitiesservice.h"
#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "idatabaseselectquerywheregroup.h"
#include "irouter.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>


ActivitiesService::ActivitiesService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
   m_activityManager = new ActivityManager(proxyConnection);
}

void ActivitiesService::init(IRouter *router)
{
    router->addRoute("/pagesCount")->on(IRequest::GET, ROUTE(pagesCount));
}

IResponse *ActivitiesService::pagesCount(IRequest *req)
{
    if(!m_proxyConnection->session()->isLoggedIn())
        return req->response(IResponse::UNAUTHORIZED);

    QVariantMap response;
    response.insert("pages", m_activityManager->pagesCount(req));

    return req->response(QVariant(response), IResponse::OK);
}

IResponse *ActivitiesService::index(IRequest *req)
{

    bool logged = false;
    QVariantMap error;

    if(!m_proxyConnection->session()->isLoggedIn())
        return req->response(IResponse::UNAUTHORIZED);


    bool ok;
    QVariantList activities = m_activityManager->getActivities(&ok, error,req);

    if(!error.empty())
    {
       return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    if(ok)
        return req->response(QVariant(activities), IResponse::OK);
    else
        return req->response(IResponse::INTERNAL_SERVER_ERROR);
}
