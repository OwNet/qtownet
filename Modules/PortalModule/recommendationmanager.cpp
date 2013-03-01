#include "recommendationmanager.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "idatabaseselectquerywheregroup.h"

#include<QSqlRecord>
#include <QSqlQuery>


RecommendationManager::RecommendationManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_activityManager = new ActivityManager(proxyConnection);
}

IResponse::Status RecommendationManager::createRecomm(IRequest *req, QString curUser_id, QVariantMap &error)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return IResponse::INTERNAL_SERVER_ERROR;

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

        return IResponse::BAD_REQUEST;
    }

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
            return IResponse::INTERNAL_SERVER_ERROR;
        }


        ///*********** TODO add create activity ***********//

        return IResponse::CREATED;

    }
    else{

        error.insert("membership in the group","required");
        return IResponse::UNAUTHORIEZED;
    }
}

IResponse::Status RecommendationManager::showRecomm(IRequest *req, QString curUser_id, QVariantMap &recommendation, QVariantMap &error)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return IResponse::INTERNAL_SERVER_ERROR;

    bool missingValue = false;

    QString id = reqJson["id"].toString();
    if(id == ""){
        error.insert("id","required");
        missingValue = true;
    }

    QString group_id = reqJson["group_id"].toString();
    if(group_id == ""){
        error.insert("group_id","required");
        missingValue = true;
    }

    if(missingValue)
        return IResponse::BAD_REQUEST;

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    bool member = m_proxyConnection->callModule(request)->body().toBool();

    if(member)
    {

        QSqlQuery query;
        query.prepare("SELECT * FROM recommendations WHERE id=:id AND group_id=:group_id");
        query.bindValue(":id",id);
         query.bindValue(":group_id",group_id);

        if(!query.exec())
            return IResponse::INTERNAL_SERVER_ERROR;


        if(query.first()) {


            recommendation.insert("id", query.value(query.record().indexOf("id")));
            recommendation.insert("user_id", query.value(query.record().indexOf("user_id")));
            recommendation.insert("group_id", query.value(query.record().indexOf("group_id")));
            recommendation.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));
            recommendation.insert("title", query.value(query.record().indexOf("title")));
            recommendation.insert("description", query.value(query.record().indexOf("description")));

        }

        return IResponse::OK;
     }
     else
     {
        error.insert("group_membership","required");
        return IResponse::UNAUTHORIEZED;
    }


}



IResponse::Status RecommendationManager::editRecomm(IRequest *req, QString curUser_id, QVariantMap &error)
{
    bool ok = true;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return IResponse::INTERNAL_SERVER_ERROR;

    bool missingValue = false;

    QString id = reqJson["id"].toString();
    if(id == ""){
        missingValue = true;
        error.insert("id","required");
    }

    QString group_id = reqJson["group_id"].toString();
    if(group_id == ""){
        missingValue = true;
        error.insert("group_id","required");
    }

    if(missingValue)
        return IResponse::BAD_REQUEST;

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isAdmin");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", reqJson["group_id"].toString());
    bool admin = m_proxyConnection->callModule(request)->body().toBool();

    QSqlQuery q;
    q.prepare("SELECT * FROM recommendations WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",curUser_id);
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

            //*********** TODO update activity ***********//

            return IResponse::OK;
        }
        else{
            return IResponse::INTERNAL_SERVER_ERROR;
        }
    }
    else{
       error.insert("admin or owner","required");
       return IResponse::UNAUTHORIEZED;
    }
}

IResponse::Status RecommendationManager::deleteRecomm(IRequest *req, QString curUser_id, QVariantMap &error)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return IResponse::INTERNAL_SERVER_ERROR;

    bool missingValue = false;

    QString id = reqJson["id"].toString();
    if(id == ""){
        missingValue = true;
        error.insert("id","required");
    }

    QString group_id = reqJson["group_id"].toString();
    if(group_id == ""){
        missingValue = true;
        error.insert("group_id","required");
    }

    if(missingValue)
        return IResponse::BAD_REQUEST;

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isAdmin");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", reqJson["group_id"].toString());
    bool admin = m_proxyConnection->callModule(request)->body().toBool();

    QSqlQuery q;
    q.prepare("SELECT * FROM recommendations WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",curUser_id);
    q.bindValue(":group_id",group_id);
    q.bindValue(":id",id);
    q.exec();

    bool owner = q.first();

    if(admin || owner){

        QObject parentObject;
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("recommendations", &parentObject);

        query->setUpdateDates(true);
        query->setType(IDatabaseUpdateQuery::Delete);

        query->singleWhere("id", id);

        if(query->executeQuery()){

            //*********** TODO delete activity ***********//
            return IResponse::OK;
        }
        else{
            return IResponse::INTERNAL_SERVER_ERROR;
        }
    }
    else{
        error.insert("admin or owner","required");
        req->response(IResponse::UNAUTHORIEZED);
    }

}
