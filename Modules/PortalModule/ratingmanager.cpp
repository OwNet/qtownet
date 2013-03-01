#include "ratingmanager.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "idatabaseselectquerywheregroup.h"

#include<QSqlRecord>
#include <QSqlQuery>

RatingManager::RatingManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_activityManager = new ActivityManager(proxyConnection);
}

IResponse::Status RatingManager::createRating(IRequest *req, QString  curUser_id, QVariantMap &error)
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

     QString value = reqJson["value"].toString();
     if(value == ""){
         missingValue = true;
         error.insert("value","required");

     }

     // missing argument
     if(missingValue){

          return IResponse::BAD_REQUEST;
     }

     // if rating already exist throw error
     QSqlQuery q;
     q.prepare("SELECT * FROM ratings WHERE absolute_uri=:absolute_uri AND user_id=:user_id");
     q.bindValue(":absolute_uri", absolute_uri);
     q.bindValue(":user_id",curUser_id);
     q.exec();

     if(q.first()){
         error.insert("error","duplicate rating");
         return IResponse::CONFLICT;
     }


     QObject parentObject;
     IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("ratings", &parentObject);

     query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

     query->setColumnValue("absolute_uri", absolute_uri);
     query->setColumnValue("val", value);
     query->setColumnValue("user_id", curUser_id);

     if(!query->executeQuery()){
         return IResponse::INTERNAL_SERVER_ERROR;
     }


     // TODO create activity

     //Activity ac;
     //ac.activity_type = Activity::RATING;
     //ac.content = value.toInt();
    // ac.group_id = 0;

     //********** TODOOOOO co s object_id *****************//
    // ac.object_id = ??

     //m_activityManager->createActivity(ac);

     return IResponse::CREATED;


 }

IResponse::Status RatingManager::showRating(IRequest *req, QVariantList &ratings, QVariantMap &error)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return IResponse::INTERNAL_SERVER_ERROR;

    QString absolute_uri = reqJson["absolute_uri"].toString();
    if(absolute_uri == ""){
        error.insert("absolute_uri","required");
        return IResponse::BAD_REQUEST;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM ratings WHERE absolute_uri=:absolute_uri");
    query.bindValue(":absolute_uri",absolute_uri);

    if(!query.exec())
        return IResponse::INTERNAL_SERVER_ERROR;

    while (query.next()) {
        QVariantMap rating;
        rating.insert("id", query.value(query.record().indexOf("id")));
        rating.insert("user_id", query.value(query.record().indexOf("user_id")));
        rating.insert("value", query.value(query.record().indexOf("value")));
        rating.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));

        ratings.append(rating);
    }

    return IResponse::OK;
}

IResponse::Status RatingManager::deleteRating(IRequest *req, QString curUser_id, QVariantMap &error)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return IResponse::INTERNAL_SERVER_ERROR;


    QString absolute_uri = reqJson["absolute_uri"].toString();
    if(absolute_uri == ""){
        error.insert("absolute_id","required");
        return IResponse::BAD_REQUEST;
    }


    QObject parentObject;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("ratings", &parentObject);
    query->setUpdateDates(true);
    query->setType(IDatabaseUpdateQuery::Delete);

    IDatabaseSelectQueryWhereGroup *where = query->whereGroup(IDatabaseSelectQuery::And);
    where->where("absolute_uri", reqJson["absolute_uri"]);
    where->where("user_id",curUser_id);

    if(!query->executeQuery()){
        return IResponse::INTERNAL_SERVER_ERROR;
    }
    else{

        //**** TODO delete activity ********//
        return IResponse::OK;
    }
}
