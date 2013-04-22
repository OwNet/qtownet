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
#include "irouter.h"
#include "isettings.h"

#include <QDir>
//#include "QSgml.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <qmath.h>
#include <QMap>
#include <QStringBuilder>
#include <QUrl>

PrefetchingService::PrefetchingService(IProxyConnection *proxyConnection, QObject* parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void PrefetchingService::init(IRouter *router)
{
    router->addRoute("/create/")
            ->on(IRequest::POST, ROUTE(create) );
    router->addRoute("/close/")
            ->on(IRequest::GET, ROUTE(close));
    router->addRoute("/done/")
            ->on(IRequest::GET, ROUTE(done));
    router->addRoute("/load/")
            ->on(IRequest::GET, ROUTE(load));
    router->addRoute("/list/")->on(IRequest::POST, ROUTE(list));
}


void PrefetchingService::registerPredictionsQuery(uint from, QStringList &urls)
{
    if (urls.count() == 0) {
         return;
    }

    int i;
    uint hash;
    QObject parent;
    QMap<uint, QString> predictions;

    // check if the prediction already exists and whether it was completed
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("completed", true);
    IDatabaseSelectQueryWhereGroup *inner = group->whereGroup(IDatabaseSelectQuery::Or);
    for (i = 0; i < urls.count(); ++i) {
        hash = m_proxyConnection->cacheId(urls.at(i));
        predictions.insert(hash, urls.at(i));
        inner->where("page_hash_to", hash);
    }

    select->select("completed");
    select->select("page_hash_to");

    while (select->next()) {    // remove completed predictions from the map
        hash = select->value("page_hash_to").toUInt();

        if (predictions.contains(hash)) {
            predictions.remove(hash);
        }
    }

    if (predictions.count() > 0) {    // if any prediction has left
        select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
        select->select("max(priority) AS maxpriority");

        int priority = DEFAULT_PRIORITY;

        if (select->next()) {
            int val = select->value("maxpriority").toInt();// + 1;
            priority = val < DEFAULT_PRIORITY ? DEFAULT_PRIORITY : (val + 1);
        }

        IDatabaseUpdateQuery *query = NULL;

        // register new prediction
        for (i = 0; i < predictions.keys().count(); ++i) {


            query = m_proxyConnection->databaseUpdateQuery("prefetch_orders",  &parent, false);
            query->setType(IDatabaseUpdateQuery::InsertOrUpdate);

            hash = predictions.keys().at(i);
            query->singleWhere("page_hash_to", hash);
            query->setColumnValue("page_hash_to", hash);
            query->setColumnValue("page_id_from", from);
            query->setColumnValue("absolute_uri", predictions.value(hash));
            query->setColumnValue("priority", priority);
            query->setUpdateDates(true);
            query->executeQuery();
        }
    }



    // update all other predictions


//    select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
//    select->singleWhere("completed", false);
//    select->select("page_id_from");
//    select->select("page_hash_to");
//    select->select("priority");

//    while (select->next()) {
//        query = m_proxyConnection->databaseUpdateQuery("prefetch_orders", &parent, false);

//        group = query->whereGroup(IDatabaseSelectQuery::And);
//        group->where("page_id_from", select->value("page_id_from"));
//        group->where("page_hash_to", select->value("page_hash_to"));

//        if (select->value("page_id_from").toInt() == from && select->value("page_hash_to").toInt() == hash)
//        {
//           // qDebug("---increment");
//            query->setColumnValue("priority", select->value("priority").toInt() + 1);
//        } else {
//           // qDebug("---decrement");
//            query->setColumnValue("priority", select->value("priority").toInt() - 1);
//        }
//        query->setUpdateDates(true);
//        query->executeQuery();
//    }


//    if (existed == false)
//    {
//        query = m_proxyConnection->databaseUpdateQuery("prefetch_orders",  &parent, false);
//        query->setColumnValue("page_id_from", from);
//        query->setColumnValue("page_hash_to", hash);
//        query->setColumnValue("absolute_uri", url);
//        query->setColumnValue("priority", DEFAULT_PRIORITY);
//        query->setUpdateDates(true);
//        query->executeQuery();
//    }
//    }
}

bool PrefetchingService::completedPrefetchingQuery(QString url)
{
    uint hash = m_proxyConnection->cacheId(url);
    QObject parent;
    IDatabaseUpdateQuery *query = NULL;
    query = m_proxyConnection->databaseUpdateQuery("prefetch_orders",  &parent,  false);
    query->singleWhere("page_hash_to", hash);
    query->setColumnValue("completed", true);
    query->setUpdateDates(true);
    return query->executeQuery();
}

bool PrefetchingService::disablePredictionQuery(uint hash)
{
    QObject parent;
    // delete prediction
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("prefetch_orders", &parent, false);
    query->singleWhere("page_id_from", hash);
    query->setType(IDatabaseUpdateQuery::Delete);
    return query->executeQuery();
}

IResponse *PrefetchingService::create(IRequest *req)
{
    bool ok = false;

    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (ok && reqJson.contains("page") && reqJson.contains("links")) {
        QString page = reqJson["page"].toString();
        QString links = reqJson["links"].toString();

        uint pageId = page.toUInt(&ok);
        QStringList predictions = links.split(',');
        if (predictions.length() > 0)
            registerPredictionsQuery(pageId, predictions);
//        if (ok) {
//            QStringList list = getPageLinks(page);

//            int count = list.length();

//            if (count > 0) {
//                QStringList predictions;
//                int xF = qFloor(count * 0.35);
//                if (xF >= 0 && xF < count)
//                    predictions.push_back(list.at(xF));

//                int yF = qFloor(count * 0.5);
//                if (yF > xF && yF < count)
//                    predictions.push_back(list.at(yF));


//                int zF = qFloor(count * 0.65);
//                if (zF > yF && zF < count)
//                    predictions.push_back(list.at(zF));


//            }
//        }
    }
    IResponse *resp = req->response(IResponse::OK);
    resp->setHeader("Access-Control-Allow-Origin","*");
    resp->setContentType("application/json");
    return resp;
}

IResponse *PrefetchingService::close(IRequest *req)
{
    if (req->hasParameter("pid")) {
        QString page = req->parameterValue("pid");
        bool ok = false;
        uint page_id = page.toUInt(&ok);
        if (ok)
        {
            disablePredictionQuery(page_id);
        }
    }
    return req->response(IResponse::OK);
}

IResponse *PrefetchingService::load(IRequest *req)
{
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

IResponse *PrefetchingService::done(IRequest *req)
{
    if (req->hasParameter("page") && !req->parameterValue("page").isEmpty()) {
        completedPrefetchingQuery(req->parameterValue("page"));
    }

    IResponse *resp = req->response(IResponse::OK);
    resp->setContentType("application/javascript");
    return  resp;
}

IResponse *PrefetchingService::list(IRequest *req) {
    QString cached = "";

    bool ok = false;

    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (ok && reqJson.contains("links")) {

        QString linksString = reqJson["links"].toString();

        QStringList links = linksString.split(',');
        if (links.size() > 0) {
            QStringList filtered = getCachedLinks(links);

            if (filtered.size() > 0) {
                int i;
                cached = "\"" + filtered.at(0) + "\"";

                for (i = 1; i < filtered.size(); ++i) {
                    cached = cached + ",\"" + filtered.at(i) + "\"";
                }
                QVariantMap ret;
                ret.insert("links",filtered);


                IResponse *resp = req->response(QVariant(ret),IResponse::OK);
                resp->setContentType("application/json");
                resp->setHeader("Access-Control-Allow-Origin","*");
                return resp;
            }


        }

    }

 //   IResponse *resp = req->response(QString("owNetAVAILABLEURIS = [%1];").arg(cached).toLatin1());
   // resp->setContentType("application/javascript");

    return req->response(IResponse::OK);
}

QStringList PrefetchingService::getCachedLinks(QStringList links) {
    uint hash = 0;
    QObject parent;
    QMap<uint, QString> map;
    QString link;

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_caches",  &parent);
    IDatabaseSelectQueryWhereGroup *group = query->whereGroup(IDatabaseSelectQuery::Or);
    int i;
    for (i = 0; i < links.size(); ++i) {
        link = links.at(i);
        hash = m_proxyConnection->cacheId(link);
        if (!map.contains(hash)) {
            group->where("cache_id", hash);
            map.insert(hash, link);
        }
    }

    query->select("cache_id");

    QStringList result;

    while (query->next()) {
        result.push_back(map.value(query->value("cache_id").toUInt()));
    }

    return result;
}

//QStringList PrefetchingService::getPageLinks(QString url)
//{
//    QObject parent;
//    QList<QSgmlTag *> elems;
//    QStringList links;

//    QUrl baseUrl(url);

//    if (baseUrl.isValid()) {
//        IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("caches", &parent);
//        query->singleWhere("absolute_uri", url);
//        query->select("id");
//        query->limit(1);

//        int i;
//        if (query->next()) {
//            bool ok = false;
//            uint id = query->value("id").toUInt(&ok);
//            if (ok) {
//                QDir dir(m_proxyConnection->settings(&parent)->value("application/data_folder_path").toString());
//                dir.cd("cache");

//                QString path = dir.absoluteFilePath(QString("%1-0.cache").arg(id));
//                QFile file(path);
//                if (file.exists()) {
//                    QSgml o(file);


//                    o.getElementsByName("a", &elems);

//                    int count = elems.length();

//                    for (i = 0; i < count; ++i ) {
//                        QSgmlTag* tag = elems.at(i);

//                        QString linkHref = tag->getArgValue("href");
//                        try {
//                            QUrl link(linkHref);
//                            if (link.isValid()) {
//                                QString newLink = QUrl(baseUrl.resolved(link)).toString();
//                                if (!links.contains(newLink)) {
//                                    links.push_back(newLink);
//                                }
//                            }
//                        }
//                        catch (...) {

//                        }
//                    }

//                    elems.clear();

//                }
//            }
//        }
//    }

//    return links;
//}
