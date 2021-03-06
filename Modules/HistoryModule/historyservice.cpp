#include "historyservice.h"

#include "irequest.h"
#include "historymodule.h"
#include "isession.h"
#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "ibus.h"
#include "idatabaseselectquery.h"
#include "idatabaseselectquerywheregroup.h"

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
    router->addRoute("/")
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

int HistoryService::registerPageQuery(IDatabaseUpdate *update, QString url, QString title)
{
    int id;

    if (update == NULL) return -1;

    IDatabaseUpdateQuery *query = update->createUpdateQuery("pages");
    query->setUpdateDates(true);
    query->setColumnValue("absolute_uri", url);
    query->setColumnValue("title", title);
    id = qHash(url);
    query->setWhere("id", id);
    return id;
}

void HistoryService::registerVisitQuery(IDatabaseUpdate *update, int user_id, int page_id)
{
    int count = 0;
    bool existed = false;
    if (update == NULL) return;
    QObject parent;

    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("user_visits_pages", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("user_id", user_id);
    group->where("page_id", page_id);

    select->select("count");
    select->limit(1);

    while (select->next()) {
        count = select->value("count").toInt();
        existed = true;
    }

    count += 1;


    IDatabaseUpdateQuery *query = NULL;
    if (existed) {
        query = update->createUpdateQuery("user_visits_pages", IDatabaseUpdateQuery::Update);
        query->setWhere("user_id", user_id);
        query->setWhere("page_id", page_id);
    }
    else {
        query = update->createUpdateQuery("user_visits_pages", IDatabaseUpdateQuery::Insert);
        query->setColumnValue("user_id", user_id);
        query->setColumnValue("page_id", page_id);
    }
    query->setUpdateDates(true);
    query->setColumnValue("count", count);
    query->setColumnValue("visited_at", QDateTime::currentDateTime().toString(Qt::ISODate));
}

void HistoryService::registerEdgeQuery(IDatabaseUpdate *update, int page_from_id, int page_to_id)
{
    if (update == NULL)
        return;

    QObject parent;
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("edges", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("page_id_from", page_from_id);
    group->where("page_id_to", page_to_id);

    if (!select->next()) {
        IDatabaseUpdateQuery *query = update->createUpdateQuery("edges", IDatabaseUpdateQuery::Insert);
        query->setUpdateDates(true);
        query->setColumnValue("page_id_from", page_from_id);
        query->setColumnValue("page_id_to", page_to_id);
    }
}

void HistoryService::registerTraverseQuery(IDatabaseUpdate *update, int user_id, int page_from_id, int page_to_id)
{

    if (update == NULL)
        return;
    QObject parent;
    int freq = 0;
    bool existed = false;
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("user_traverses_edges", &parent);
    IDatabaseSelectQueryWhereGroup *group = select->whereGroup(IDatabaseSelectQuery::And);
    group->where("edge_page_id_from", page_from_id);
    group->where("edge_page_id_to", page_to_id);
    group->where("user_id", user_id);

    select->select("frequency");
    select->limit(1);

    while (select->next()) {
        freq = select->value("frequency").toInt();
        existed = true;
    }

    freq += 1;
    IDatabaseUpdateQuery *query = NULL;
    if (existed) {
        query = update->createUpdateQuery("user_traverses_edges", IDatabaseUpdateQuery::Update);
        query->setWhere("edge_page_id_from", page_from_id);
        query->setWhere("edge_page_id_to", page_to_id);
        query->setWhere("user_id", user_id);

    }
    else {
        query = update->createUpdateQuery("user_traverses_edges", IDatabaseUpdateQuery::Insert);
        query->setColumnValue("edge_page_id_from", page_from_id);
        query->setColumnValue("edge_page_id_to", page_to_id);
        query->setColumnValue("user_id", user_id);
    }
    query->setUpdateDates(true);
    query->setColumnValue("frequency", freq);
}

IResponse *HistoryService::visit(IRequest *req)
{
    QObject parent;
    int idfrom = -1;
    int idto = -1;
    bool referrer = false;
    if (req->hasParameter("page")) {

        IDatabaseUpdate *update = m_proxyConnection->databaseUpdate(&parent);

        QString page = req->parameterValue("page");
        if (page.contains("prefetch.ownet/api"))
            return req->response(IResponse::NOT_FOUND);

        int user_id = -1;
        bool t = false;

        if (m_proxyConnection->session(&parent)->isLoggedIn()) {
            user_id = m_proxyConnection->session(&parent)->value("logged").toInt(&t);
            if (t == false) user_id = -1;
        }

       // m_module->prefetchJob()->registerPage(id,  page);

        idto = registerPageQuery(update, page, "Titulok stranky");

        if (user_id != -1) {
            registerVisitQuery(update, user_id, idto);
        }


        if (req->hasParameter("ref")) {
            page = req->parameterValue("ref");
            if (!page.isEmpty() && !page.contains("prefetch.ownet/api")) {

                idfrom = registerPageQuery(update, page, "Titulok stranky");
                referrer = true;
  //              m_module->prefetchJob()->removePage(page);
            }
        }


        int a = update->execute();
        if(!a) {

            if (referrer) {
                update = m_proxyConnection->databaseUpdate(&parent);

                registerEdgeQuery(update, idfrom, idto);

                if (user_id != -1) {
                    registerTraverseQuery(update, user_id, idfrom, idto);
                }

                update->execute();
            }
            return req->response(QString("owNetPAGEID = %1;").arg(QString::number(idto)).toLatin1());
        }
    }
    return req->response(IResponse::NOT_FOUND);
}



QByteArray *HistoryService::close(IBus *, IRequest *req)
{
    if (req->hasParameter("page")) {
        QString page = req->parameterValue("page");
        bool ok = false;
        int pageId = page.toInt(&ok, 10);
        if (ok)
        {
           // m_module->prefetchJob()->removePage(pageId);
            //MessageHelper::debug(QString("PREFETCH: Removed %0").arg(page));
        }
    }
    return NULL;
}
