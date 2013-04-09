#include "recommendationmanager.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "idatabaseselectquerywheregroup.h"
#include "isynceddatabaseupdatequery.h"

#include<QSqlRecord>
#include <QSqlQuery>
#include <QUuid>

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

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    QString member = m_proxyConnection->callModule(request)->body().toMap().value("member").toString();

    // overit ci je userom skupiny do ktorej chce pridat recomm
    if(member == "1"){

        // if rating already exist throw error
        QSqlQuery q;
        q.prepare("SELECT * FROM recommendations WHERE absolute_uri=:absolute_uri AND user_id=:user_id");
        q.bindValue(":absolute_uri", absolute_uri);
        q.bindValue(":user_id",curUser_id);
        q.exec();

        if(q.first()){
            error.insert("error","duplicate recommendations");
            return IResponse::CONFLICT;
        }

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
        QString uid = query->syncedQuery()->lastUid();

        // create activity

        Activity ac;

        //username is solved inside createActivity method
        ac.activity_type = Activity::RECOMMENDATION;
        ac.content = absolute_uri + QString(";") + title;
        ac.group_id = group_id;
        ac.object_id = uid;
        ac.user_id = curUser_id;

        m_activityManager->createActivity(ac);

        return IResponse::CREATED;

    }
    else{

        error.insert("membership in the group","required");
        return IResponse::UNAUTHORIEZED;
    }
}

IResponse::Status RecommendationManager::showRecomm(IRequest *req, const QString &uid ,QString curUser_id, QVariantMap &recommendation, QVariantMap &error)
{

    bool missingValue = false;

    QString group_id = req->parameterValue("group_id");
    if(group_id == ""){
        error.insert("group_id","required");
        missingValue = true;
    }



    if(missingValue)
        return IResponse::BAD_REQUEST;

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    QString member = m_proxyConnection->callModule(request)->body().toMap().value("member").toString();

    // overit ci je userom skupiny do ktorej chce pridat recomm
    if(member == "1")
    {

        QSqlQuery query;
        query.prepare("SELECT * FROM recommendations WHERE uid=:uid AND group_id=:group_id");
        query.bindValue(":uid",uid);
         query.bindValue(":group_id",group_id);

        if(!query.exec())
            return IResponse::INTERNAL_SERVER_ERROR;


        if(query.first()) {


            recommendation.insert("id", query.value(query.record().indexOf("_id")));
            recommendation.insert("user_id", query.value(query.record().indexOf("user_id")));
            recommendation.insert("group_id", query.value(query.record().indexOf("group_id")));
            recommendation.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));
            recommendation.insert("title", query.value(query.record().indexOf("title")));
            recommendation.insert("description", query.value(query.record().indexOf("description")));
            recommendation.insert("uid", query.value(query.record().indexOf("uid")));


        }

        return IResponse::OK;
     }
     else
     {
        error.insert("group_membership","required");
        return IResponse::UNAUTHORIEZED;
    }


}



IResponse::Status RecommendationManager::editRecomm(IRequest *req, const QString &uid, QString curUser_id, QVariantMap &error)
{
    bool ok = true;


    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok){
        error.insert("json","parse_error");
        return IResponse::BAD_REQUEST;
    }

    bool missingValue = false;

    QString group_id = reqJson["group_id"].toString();
    if(group_id == ""){
        missingValue = true;
        error.insert("group_id","required");
    }

    QString absolute_uri = reqJson["absolute_uri"].toString();
    if(absolute_uri == ""){
        missingValue = true;
        error.insert("absolute_uri","required");
    }

    if(missingValue)
        return IResponse::BAD_REQUEST;

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "groups", "isAdmin");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    QString admin = m_proxyConnection->callModule(request)->body().toMap().value("admin").toString();

    QSqlQuery q;
    q.prepare("SELECT * FROM recommendations WHERE uid=:uid AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",curUser_id);
    q.bindValue(":group_id",reqJson["group_id"].toString());
    q.bindValue(":uid", uid);
    q.exec();

    bool owner = q.first();

    if(admin == "1" || owner){
        QObject parent;

        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("recommendations", &parent);
        query->setUpdateDates(true);
        query->singleWhere("uid", uid);

        if(reqJson.contains("title"))
            query->setColumnValue("title", reqJson["title"].toString());

        if(reqJson.contains("description"))
            query->setColumnValue("description", reqJson["description"].toString());

        if(query->executeQuery()){

            //edit activity
            if(reqJson.contains("title"))
                m_activityManager->editActivity( uid, absolute_uri + ";" + reqJson["title"].toString());
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

IResponse::Status RecommendationManager::deleteRecomm(IRequest *req, const QString &uid,  QString curUser_id, QVariantMap &error)
{
     bool missingValue = false;

    QString group_id = req->parameterValue("group_id");
    if(group_id == ""){
        error.insert("group_id_parameter","required");
        missingValue = true;
    }


    if(missingValue)
        return IResponse::BAD_REQUEST;

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isAdmin");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);
    bool admin = m_proxyConnection->callModule(request)->body().toBool();

    QSqlQuery q;
    q.prepare("SELECT * FROM recommendations WHERE uid=:uid AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",curUser_id);
    q.bindValue(":group_id",group_id);
    q.bindValue(":uid",uid);
    q.exec();

    bool owner = q.first();

    if(admin || owner){

        QObject parentObject;
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("recommendations", &parentObject);

        query->setUpdateDates(true);
        query->setType(IDatabaseUpdateQuery::Delete);

        query->singleWhere("uid", uid);

        if(query->executeQuery()){

            m_activityManager->deleteActivity(uid);
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
