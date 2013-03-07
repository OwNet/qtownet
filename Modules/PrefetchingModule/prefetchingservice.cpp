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

#include <QDir>
#include "QSgml.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <qmath.h>
#include <QSettings>
#include <QMap>

PrefetchingService::PrefetchingService(IProxyConnection *proxyConnection, QObject* parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void PrefetchingService::init(IRouter *router)
{
    router->addRoute("/create/")
            ->on(IRequest::GET, ROUTE(create) );
    router->addRoute("/close/")
            ->on(IRequest::GET, ROUTE(close));
    router->addRoute("/done/")
            ->on(IRequest::GET, ROUTE(done));
    router->addRoute("/load/")
            ->on(IRequest::GET, ROUTE(load));
}


void PrefetchingService::registerPredictionsQuery(int from, QStringList &urls)
{
    if (urls.count() == 0) {
         return;
    }

    int i;
    int hash;
    QObject parent;
    QMap<int, QString> predictions;

    // check if the prediction already exists and whether it was completed
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::Or);
    for (i = 0; i < urls.count(); ++i) {
        hash = qHash(QUrl(urls.at(i)));
        predictions.insert(hash, urls.at(i));
        group->where("page_hash_to", hash);
    }


    //"page_hash_to", hash);
    bool completed = false;
    select->select("completed");
    select->select("page_hash_to");

    while (select->next()) {    // remove completed predictions from the map
        completed = select->value("completed").toBool();
        hash = select->value("page_hash_to").toInt();

        if (completed && predictions.contains(hash)) {
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
    //        query->setColumnValue("page_hash_to", hash);
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
    query->singleWhere("page_id_from", hash);
    query->setType(IDatabaseUpdateQuery::Delete);
    return query->executeQuery();
}

IResponse *PrefetchingService::create(IRequest *req)
{
    if (req->hasParameter("page") && req->hasParameter("pid")) {
        QString page = req->parameterValue("page");
        QString padeIdString = req->parameterValue("pid");
        bool ok = false;
        int pageId = padeIdString.toInt(&ok);

        if (ok) {
            QStringList list = getTopLinks(page);

            int count = list.length();

            if (count > 0) {
                QStringList predictions;
                int xF = qFloor(count * 0.35);
                if (xF >= 0 && xF < count)
                    predictions.push_back(list.at(xF));

                int yF = qFloor(count * 0.5);
                if (yF > xF && yF < count)
                    predictions.push_back(list.at(yF));


                int zF = qFloor(count * 0.65);
                if (zF > yF && zF < count)
                    predictions.push_back(list.at(zF));

                registerPredictionsQuery(pageId, predictions);
            }
        }
    }
    return req->response(IResponse::OK);
}


IResponse *PrefetchingService::close(IRequest *req)
{
    if (req->hasParameter("pid")) {
        QString page = req->parameterValue("pid");
        bool ok = false;
        int page_id = page.toInt(&ok, 10);
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

    return  req->response(IResponse::OK);
}


QStringList PrefetchingService::getTopLinks(QString url)
{
    QObject parent;
    QList<QSgmlTag *> elems;
    QStringList links;

    QUrl baseUrl(url);

    if (baseUrl.isValid()) {
        IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("caches", &parent);
        query->singleWhere("absolute_uri", url);
        query->select("id");
        query->limit(1);

        int i;
        if (query->next()) {
            bool ok = false;
            int id = query->value("id").toInt(&ok);
            if (ok) {
                QDir dir(m_proxyConnection->settings(&parent)->value("application/data_folder_path").toString());
                dir.cd("cache");

                QString path = dir.absoluteFilePath(QString("%1-0.cache").arg(id));
                QFile file(path);
                if (file.exists()) {
                    QSgml o(file);


                    o.getElementsByName("a", &elems);

                    int count = elems.length();

                    for (i = 0; i < count; ++i ) {
                        QSgmlTag* tag = elems.at(i);

                        QString linkHref = tag->getArgValue("href");
                        try {
                            QUrl link(linkHref);
                            if (link.isValid()) {
                                QString newLink = QUrl(baseUrl.resolved(link)).toString();
                                if (!links.contains(newLink)) {
                                    links.push_back(newLink);
                                }
                            }
                        }
                        catch (...) {

                        }
                    }

                    elems.clear();

                }
            }
        }
    }

    return links;
}
