#include "historyservice.h"

#include "irequest.h"
#include "isession.h"
#include "irequest.h"
#include "irouter.h"
#include "iproxyconnection.h"
#include "idatabaseselectquery.h"
#include "idatabaseselectquerywheregroup.h"
#include "idatabaseupdatequery.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

HistoryService::HistoryService(IProxyConnection *proxyConnection, QObject* parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void HistoryService::init(IRouter *router)
{
    router->addRoute("/visit/")
            ->on(IRequest::GET, ROUTE(visit) );
}


//QByteArray *HistoryService::processRequest(IBus *bus, IRequest *req)
//{
// //   MessageHelper::debug(QString("PREFETCH %1").arg(req->relativeUrl()));

//    //QVariantMap response;
//    QByteArray *ret = NULL;
//    if (req->action() == "visit") {
//        ret = visit(bus,req);
//    }

//    if (ret) {
//        return ret;
//    }

//    return new QByteArray("{ ERROR : 'PROBLEM'}");
//}

bool HistoryService::registerPageQuery(QString url, QString title, uint &id)
{
    QObject parent;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("pages", &parent, false);
    query->setUpdateDates(true);
    query->setColumnValue("absolute_uri", url);
    query->setColumnValue("title", title);
    id = m_proxyConnection->cacheId(url);
    query->setColumnValue("id", id);
    query->singleWhere("id", id);
    return query->executeQuery();
}

bool HistoryService::registerVisitQuery(QString user_id, uint page_id)
{
    int count = 0;
    QObject parent;




    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("user_visits_pages", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("user_id", user_id);
    group->where("page_id", page_id);

    select->select("count");
    select->limit(1);

    if (select->next()) {
        count = select->value("count").toInt();
    }

    count += 1;


    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("user_visits_pages", &parent, false);
    IDatabaseSelectQueryWhereGroup *where = query->whereGroup(IDatabaseSelectQuery::And);
    where->where("user_id", user_id);
    where->where("page_id", page_id);
    query->setColumnValue("user_id", user_id);
    query->setColumnValue("page_id", page_id);
    query->setColumnValue("count", count);
    //query->setColumnValue("visited_at", QDateTime::currentDateTime().toString(Qt::ISODate));
    query->setUpdateDates(true);
    return query->executeQuery();
}

bool HistoryService::registerEdgeQuery(uint page_from_id, uint page_to_id)
{
    QObject parent;
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("edges", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("page_id_from", page_from_id);
    group->where("page_id_to", page_to_id);

    if (!select->next()) {
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("edges", &parent, false);
        query->setUpdateDates(true);
        query->setColumnValue("page_id_from", page_from_id);
        query->setColumnValue("page_id_to", page_to_id);
        return query->executeQuery();
    }
    return true;
}

bool HistoryService::registerTraverseQuery(QString user_id, uint page_from_id, uint page_to_id)
{

    QObject parent;
    int freq = 0;
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("user_traverses_edges", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("edge_page_id_from", page_from_id);
    group->where("edge_page_id_to", page_to_id);
    group->where("user_id", user_id);

    select->select("frequency");
    select->limit(1);

    if (select->next()) {
        freq = select->value("frequency").toInt();
    }

    freq += 1;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("user_traverses_edges", &parent, false);
    IDatabaseSelectQueryWhereGroup *where = query->whereGroup(IDatabaseSelectQuery::And);
    where->where("edge_page_id_from", page_from_id);
    where->where("edge_page_id_to", page_to_id);
    where->where("user_id", user_id);
    query->setColumnValue("edge_page_id_from", page_from_id);
    query->setColumnValue("edge_page_id_to", page_to_id);
    query->setColumnValue("user_id", user_id);
    query->setUpdateDates(true);
    query->setColumnValue("frequency", freq);
    return query->executeQuery();
}

IResponse *HistoryService::visit(IRequest *req)
{
    QObject parent;
    uint idfrom = 0;
    uint idto = 0;

    bool success = true;
    if (req->hasParameter("page")) {

        QString page = req->parameterValue("page");
        if (page.contains("prefetch.ownet/api"))
            return req->response(IResponse::NOT_FOUND);

        int uid = -1;
        QString user_id = "NULL";
        bool t = false;

        if (m_proxyConnection->session(&parent)->isLoggedIn()) {
            uid = m_proxyConnection->session(&parent)->value("logged").toInt(&t);
            if (t == true && uid != -1) {
                user_id = QString::number(uid);
            }
        }

        // m_module->prefetchJob()->registerPage(id,  page);

        if (!registerPageQuery(page, "Titulok stranky", idto))
            success = false;


        if (!registerVisitQuery(user_id, idto))
            success = false;



        if (req->hasParameter("ref")) {
            page = req->parameterValue("ref");
            if (!page.isEmpty() && !page.contains("prefetch.ownet/api")) {

                if (registerPageQuery(page, "Titulok stranky", idfrom)) {
                    if (registerEdgeQuery(idfrom, idto)) {
                        registerTraverseQuery(user_id, idfrom, idto);
                    }
                }
                else {
                    success = false;
                }
                // m_module->prefetchJob()->removePage(page);
            }
        }


        if (success) {
            return req->response(QString("owNetPAGEID = %1;").arg(QString::number(idto)).toLatin1());
        }
    }
    return req->response(IResponse::NOT_FOUND);
}



//QByteArray *HistoryService::close(IBus *, IRequest *req)
//{
//    if (req->hasParameter("page")) {
//        QString page = req->parameterValue("page");
//        bool ok = false;
//        int pageId = page.toInt(&ok, 10);
//        if (ok)
//        {
//           // m_module->prefetchJob()->removePage(pageId);
//            //MessageHelper::debug(QString("PREFETCH: Removed %0").arg(page));
//        }
//    }
//    return NULL;
//}
