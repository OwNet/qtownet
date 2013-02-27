#include "ratingsservice.h"
#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "idatabaseselectquerywheregroup.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

RatingsService::RatingsService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void RatingsService::init(IRouter *router)
{

}

// create element
IResponse *RatingsService::create(IRequest *req)
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

    QString value = reqJson["value"].toString();
    if(value == ""){
        missingValue = true;
        error.insert("value","required");

    }
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == ""){
        missingValue = true;
        error.insert("error","not logged in");

    }
    // missing argument

    if(missingValue){

         return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    // if rating already exist throw error
    QSqlQuery q;
    q.prepare("SELECT * FROM ratings WHERE absolute_uri=:absolute_uri AND user_id=:user_id");
    q.bindValue(":absolute_uri", absolute_uri);
    q.bindValue(":user_id",curUser_id);
    q.exec();

    if(q.first()){
        error.insert("error","duplicate rating");
        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }


    QObject parentObject;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("messages", &parentObject);

    query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

    query->setColumnValue("absolute_uri", absolute_uri);
    query->setColumnValue("val", value);
    query->setColumnValue("user_id", curUser_id);

    if(!query->executeQuery()){
        return req->response(QVariant(error), IResponse::INTERNAL_SERVER_ERROR);
    }

    return req->response(IResponse::CREATED);


}

IResponse *RatingsService::index(IRequest *req)
{
    bool logged = false;
    QVariantMap error;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == ""){
        logged = true;
        error.insert("error","not logged in");

    }
    // if not logged in
    if(logged){

        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM ratings");
    query.exec();

    QVariantList ratings;

    while (query.next()) {
        QVariantMap rating;
        rating.insert("id", query.value(query.record().indexOf("id")));
        rating.insert("user_id", query.value(query.record().indexOf("user_id")));
        rating.insert("value", query.value(query.record().indexOf("value")));
        rating.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));

        ratings.append(rating);
    }

    return req->response(QVariant(ratings), IResponse::BAD_REQUEST);
}

IResponse *RatingsService::show(IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    bool logged = false;
    QVariantMap error;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == ""){
        logged = true;
        error.insert("error","not logged in");

    }
    // if not logged in
    if(logged){

        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    QString absolute_uri = reqJson["absolute_uri"].toString();
    if(absolute_uri == ""){
        error.insert("absolute_uri","required");
        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM ratings WHERE absolute_uri=:absolute_uri");
    query.bindValue(":absolute_uri",absolute_uri);
    query.exec();

    QVariantList ratings;

    while (query.next()) {
        QVariantMap rating;
        rating.insert("id", query.value(query.record().indexOf("id")));
        rating.insert("user_id", query.value(query.record().indexOf("user_id")));
        rating.insert("value", query.value(query.record().indexOf("value")));
        rating.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));

        ratings.append(rating);
    }

    return req->response(QVariant(ratings), IResponse::OK);
}


IResponse *RatingsService::del(IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    bool logged = false;
    QVariantMap error;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == ""){
        logged = true;
        error.insert("error","not logged in");

    }
    // if not logged in
    if(logged){

       return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    QString absolute_uri = reqJson["idi"].toString();
    if(absolute_uri == ""){
        error.insert("id","required");
        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }


    QObject parentObject;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("ratings", &parentObject);
    query->setUpdateDates(true);

    IDatabaseSelectQueryWhereGroup *where = query->whereGroup(IDatabaseSelectQuery::And);
    where->where("id", reqJson["id"]);
    where->where("user_id",curUser_id);

    if(!query->executeQuery()){
        return req->response(IResponse::OK);
    }

    else{

        return req->response(IResponse::INTERNAL_SERVER_ERROR);

    }

}

