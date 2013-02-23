#include "recommendationsservice.h"

#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "isession.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

RecommendationsService::RecommendationsService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void RecommendationsService::init(IRouter *router)
{

}

// create element
IResponse *RecommendationsService::create(IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QVariantMap error;

    bool missingValue = false;

    QString absolute_uri = reqJson["absolute_uri"].toString();
    if(absolute_uri == ""){
        missingValue = true;
        error.insert("absolute_uri","required");
    }

    QString description = reqJson["description"].toString();
    if(description == ""){
        missingValue = true;
        error.insert("description","required");

    }

    QString title = reqJson["title"].toString();
    if(title == ""){
        missingValue = true;
        error.insert("title","required");

    }

    QString group_id = reqJson["group_id"].toString();
    if(group_id == ""){
        missingValue = true;
        error.insert("group_id","required");
    }

    // missing argument

    if(missingValue){

        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    bool member = m_proxyConnection->callModule(request)->body().toBool();

    // overit ci je userom skupiny do ktorej chce pridat recomm
    if(member){


        QObject parentObject;
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("recommendations", &parentObject);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        query->setColumnValue("title", title);
        query->setColumnValue("absolute_uri", absolute_uri);
        query->setColumnValue("group_id", group_id);
        query->setColumnValue("description", description);
        query->setColumnValue("user_id", curUser_id);

        if(!query->executeQuery()){
            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }
        return req->response(IResponse::CREATED);

    }
    else{

        QVariantMap err;
        err.insert("membership of the group","required");
        return req->response(QVariant(err), IResponse::BAD_REQUEST);
    }


}

IResponse *RecommendationsService::index(IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QString group_id = reqJson["group_id"].toString();
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    bool member = m_proxyConnection->callModule(request)->body().toBool();

    // overit ci je userom skupiny do ktorej chce pridat recomm
    if(member){
        QSqlQuery query;

        query.prepare("SELECT * FROM recommendations WHERE group_id = :group_id");
        query.bindValue(":group_id",group_id);
        query.exec();

        QVariantList recomms;

        while (query.next()) {
            QVariantMap recomm;
            recomm.insert("id", query.value(query.record().indexOf("id")));
            recomm.insert("title", query.value(query.record().indexOf("title")));
            recomm.insert("description", query.value(query.record().indexOf("description")));
            recomm.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));

            recomms.append(recomm);
        }

        return req->response(QVariant(recomms), IResponse::OK);

    }

    return req->response(IResponse::BAD_REQUEST);
}

IResponse *RecommendationsService::edit(IRequest *req)
{
    bool ok = true;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isAdmin");
    request->setParamater("user_id", reqJson["user_id"].toString());
    request->setParamater("group_id", reqJson["group_id"].toString());
    bool admin = m_proxyConnection->callModule(request)->body().toBool();

    QSqlQuery q;
    q.prepare("SELECT * FROM recommendations WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",reqJson["user_id"].toString());
    q.bindValue(":group_id",reqJson["group_id"].toString());
    q.bindValue(":id",reqJson["id"]);
    q.exec();

    bool owner = q.first();

    if(admin || owner){
        QObject parent;

        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("groups", &parent);
        query->setUpdateDates(true);
        query->singleWhere("id", reqJson["id"]);

        if(reqJson["title"] != "")
            query->setColumnValue("title", reqJson["title"].toString());

        if(reqJson["description"] != "")
            query->setColumnValue("description", reqJson["description"].toString());

        if(query->executeQuery()){
            return req->response(IResponse::OK);
        }
        else{
            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }
    }
    else{
       return req->response(IResponse::UNAUTHORIEZED);
    }

}

IResponse *RecommendationsService::del(IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isAdmin");
    request->setParamater("user_id", reqJson["user_id"].toString());
    request->setParamater("group_id", reqJson["group_id"].toString());
    bool admin = m_proxyConnection->callModule(request)->body().toBool();

    QSqlQuery q;
    q.prepare("SELECT * FROM recommendations WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",reqJson["user_id"].toString());
    q.bindValue(":group_id",reqJson["group_id"].toString());
    q.bindValue(":id",reqJson["id"]);
    q.exec();

    bool owner = q.first();

    if(admin || owner){

        QObject parentObject;
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("messages", &parentObject);

        query->setUpdateDates(true);
        query->setType(IDatabaseUpdateQuery::Delete);

        query->singleWhere("id", reqJson["id"]);

        if(query->executeQuery()){
            return req->response(IResponse::OK);
        }
        else{
            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }
    }
    else{
       req->response(IResponse::UNAUTHORIEZED);
    }

}


