#include "downloadordersservice.h"

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
#include "irouter.h"

#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <qmath.h>
#include <QSettings>
#include <QMap>
#include <QStringBuilder>


DownloadOrdersService::DownloadOrdersService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{

}

void DownloadOrdersService::init(IRouter *router)
{
    router->addRoute("/allPagesCount")->on(IRequest::GET, ROUTE(allPagesCount));
}

IResponse *DownloadOrdersService::del(IRequest *req, uint id)
{
    QObject parent;


    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    query->select("page_hash_to");
    query->singleWhere("page_hash_to", id);
    query->limit(1);
    if (query->next()) {
        id = query->value("page_hash_to").toInt();
        IDatabaseUpdateQuery *update = m_proxyConnection->databaseUpdateQuery("prefetch_orders", &parent, false);
        update->singleWhere("page_hash_to", id);
        update->setType(IDatabaseUpdateQuery::Delete);
        update->executeQuery();
        return req->response(IResponse::OK);
    }
    else {
        return req->response(IResponse::NOT_FOUND);
    }

}

IResponse *DownloadOrdersService::allPagesCount(IRequest *req)
{
    QObject parent;

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    query->select("COUNT(*) as count");

    if (query->next()) {
        QVariantMap response;
        int x = query->value("count").toInt();
        response.insert("pages",qCeil(x/(double)PER_PAGE));
        return req->response(QVariant(response),IResponse::OK);
    }
    else
        return req->response(IResponse::INTERNAL_SERVER_ERROR);
}


IResponse *DownloadOrdersService::edit(IRequest *req, uint )
{
    return req->response(IResponse::NOT_IMPLEMENTED);
}


IResponse *DownloadOrdersService::index(IRequest *req)
{
    QObject parent;
    int page = 1;
    bool ok = false;
    if (req->hasParameter("page")) {
        page = req->parameterValue("page").toInt(&ok);
        if (ok == false) {
            page = 1;
        }
    }

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    query->select("absolute_uri");
    query->select("completed");
    query->select("date_updated");
    query->select("page_hash_to");
    query->offset((page - 1) * PER_PAGE);
    query->limit(PER_PAGE);
    query->orderBy("priority", IDatabaseSelectQuery::Descending);

    QVariantList orders;


    while (query->next()) {
        QVariantMap order;
        order.insert("id", query->value("page_hash_to"));
        QString link = query->value("absolute_uri").toString();
        if (link.length() > 60)
            link = link.left(60).append("...");
        order.insert("absolute_uri", link);
        order.insert("completed", query->value("completed"));
        order.insert("date", query->value("date_updated"));

        orders.append(order);
    }

    return req->response(QVariant(orders), IResponse::OK);
}


IResponse *DownloadOrdersService::show(IRequest *req, uint )
{
    return req->response(IResponse::NOT_IMPLEMENTED);
}


IResponse *DownloadOrdersService::create(IRequest *req)
{
    bool ok = false;

    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (ok && reqJson.contains("url") && reqJson.contains("title")) {
        QString link = reqJson["url"].toString();
        QString title = reqJson["title"].toString();
        if (registerOrderQuery(title, link)) {
            IResponse *resp = req->response(IResponse::OK);
            resp->setHeader("Access-Control-Allow-Origin","*");
            resp->setContentType("application/json");
            return resp;
        }
    }
    IResponse *resp = req->response(IResponse::BAD_REQUEST);
    resp->setHeader("Access-Control-Allow-Origin","*");
    resp->setContentType("application/json");
    return resp;
}

bool DownloadOrdersService::registerOrderQuery(QString title, QString url) {
    if (!url.startsWith("http://") || title.isEmpty()) {
        return false;
    }

    QObject parent;
    // check if the prediction already exists and whether it was completed
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("completed", "TRUE");

    uint hash = m_proxyConnection->cacheId(url);
    group->where("page_hash_to", hash);
    select->select("page_hash_to");

    if (select->next()) {   // TODO old download
        return true;
    }


    int priority = ORDER_PRIORITY;

    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("prefetch_orders",  &parent, false);
    query->setType(IDatabaseUpdateQuery::InsertOrUpdate);

    query->singleWhere("page_hash_to", hash);
    query->setColumnValue("page_hash_to", hash);
    query->setColumnValue("absolute_uri", url);
    query->setColumnValue("priority", priority);
    query->setUpdateDates(true);
    query->executeQuery();

    return true;
}
