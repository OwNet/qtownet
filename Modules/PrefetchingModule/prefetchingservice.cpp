#include "prefetchingservice.h"

#include "irequest.h"
#include "prefetchingmodule.h"
#include "prefetchingjob.h"
#include "isession.h"
#include "iresponse.h"
#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "idatabaseselectquery.h"
#include "idatabaseselectquerywheregroup.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>

PrefetchingService::PrefetchingService(IProxyConnection *proxyConnection, QObject* parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void PrefetchingService::init(IRouter *router)
{
    router->addRoute("/link/")
            ->on(IRequest::GET, ROUTE(link) );
    router->addRoute("/close/")
            ->on(IRequest::GET, ROUTE(close));
    router->addRoute("/done/")
            ->on(IRequest::GET, ROUTE(done));
    router->addRoute("/load/")
            ->on(IRequest::GET, ROUTE(load));

}

void PrefetchingService::registerPredictionQuery(int from, QString url)
{
    int hash = qHash(url);
    bool existed = false;
    QObject parent;

    // check if the prediction already exists and whether it was completed
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("page_id_from", from);
    group->where("page_hash_to", hash);
    bool completed = false;
    select->select("completed");
    select->limit(1);

    if (select->next()) {
      //  qDebug("----- hey hey hey ------");
        existed = true;
        completed = select->value("completed").toBool();
    }

    if (completed) {
        return;
    }


    // update all other predictions
    IDatabaseUpdateQuery *query = NULL;

    select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    select->singleWhere("completed", false);
    select->select("page_id_from");
    select->select("page_hash_to");
    select->select("priority");

    while (select->next()) {
        query = m_proxyConnection->databaseUpdateQuery("prefetch_orders", &parent, false);
        group = query->whereGroup(IDatabaseSelectQuery::And);
        group->where("page_id_from", select->value("page_id_from"));
        group->where("page_hash_to", select->value("page_hash_to"));

        if (select->value("page_id_from") == from && select->value("page_hash_to").toInt() == hash)
        {
           // qDebug("---increment");
            query->setColumnValue("priority", select->value("priority").toInt() + 1);
        } else {
           // qDebug("---decrement");
            query->setColumnValue("priority", select->value("priority").toInt() - 1);
        }
        query->setUpdateDates(true);
        query->executeQuery();
    }

    // register new prediction
    query = m_proxyConnection->databaseUpdateQuery("prefetch_orders",  &parent, false);
    query->setColumnValue("page_id_from", from);
    query->setColumnValue("page_hash_to", hash);
    query->setColumnValue("absolute_uri", url);
    query->setColumnValue("priority", DEFAULT_PRIORITY);
    query->setUpdateDates(true);
    query->executeQuery();
}

bool PrefetchingService::completedPrefetchingQuery(QString url)
{
    int hash = qHash(url);
    QObject parent;
    IDatabaseUpdateQuery *query = NULL;
    query = m_proxyConnection->databaseUpdateQuery("prefetch_orders",  &parent,  false);
    query->singleWhere("page_hash_to", hash);
    query->setColumnValue("completed", true);
    query->setUpdateDates(true);
    return query->executeQuery();
}

bool PrefetchingService::disablePredictionQuery(int hash)
{
    QObject parent;
    // delete prediction
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("prefetch_orders", &parent, false);
    query->singleWhere("page_hash_to", hash);
    query->setType(IDatabaseUpdateQuery::Delete);
    return query->executeQuery();
}

IResponse *PrefetchingService::link(IRequest *req)
{
    qDebug("------------Hey, I just met you.");
    if (req->hasParameter("from") && req->hasParameter("to")) {
        QString page = req->parameterValue("from");
        bool ok = false;
        int page_id = page.toInt(&ok);
        QString target = req->parameterValue("to");
        if (ok && !target.isEmpty()) {
            registerPredictionQuery(page_id, target);
            //update->execute();
        }
    }
    return req->response(IResponse::OK);
}


IResponse *PrefetchingService::close(IRequest *req)
{
    if (req->hasParameter("page")) {
        QString page = req->parameterValue("page");
        bool ok = false;
        int page_id = page.toInt(&ok, 10);
        if (ok)
        {
            disablePredictionQuery(page_id);
            //MessageHelper::debug(QString("PREFETCH: Removed %0").arg(page));
        }
    }
    return req->response(IResponse::OK);
}

IResponse *PrefetchingService::load(IRequest *req) {
    IResponse *response = NULL;
    if (req->hasParameter("page") && !req->parameterValue("page").isEmpty()) {
        //req->setContentType("text/html");
        response = req->response(QString("<html><body onload=\"document.links[0].click();\"><a id=\"clickme\" href=\"%1\">prefetch this</a></body></html>").arg(req->parameterValue("page")).toLatin1());
    } else {
        response = req->response(IResponse::OK);
    }
    response->setContentType("text/html");
    return response;
}


IResponse *PrefetchingService::done(IRequest *req) {

    if (req->hasParameter("page") && !req->parameterValue("page").isEmpty()) {
        completedPrefetchingQuery(req->parameterValue("page"));
    }

    return  req->response(IResponse::OK);
}
