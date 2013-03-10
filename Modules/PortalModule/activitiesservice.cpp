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
    router->addRoute("/allPagesCount")->on(IRequest::GET, ROUTE(pagesCount));

}

IResponse *ActivitiesService::pagesCount(IRequest *req)
{
    if(!m_proxyConnection->session()->isLoggedIn())
        req->response(IResponse::UNAUTHORIEZED);

    QVariantMap response;
    response.insert("pages", m_activityManager->PagesCount(req));

    return req->response(QVariant(response), IResponse::OK);
}

IResponse *ActivitiesService::index(IRequest *req)
{

    bool logged = false;
    QVariantMap error;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == ""){
        logged = true;
        error.insert("error","not logged in");

    }
    // if not logged in
    if(logged){
        return req->response(QVariant(error), IResponse::UNAUTHORIEZED);
    }

    bool ok;
    QVariantList activities = m_activityManager->getActivities(&ok, req);

    if(ok)
        return req->response(QVariant(activities), IResponse::OK);
    else
        return req->response(IResponse::INTERNAL_SERVER_ERROR);
}

