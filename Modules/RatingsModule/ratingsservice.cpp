#include "ratingsservice.h"
#include "irequest.h"
#include "idatabaseupdate.h"
#include "ibus.h"
#include "iproxyconnection.h"
#include "isession.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

RatingsService::RatingsService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

// create element
QVariant *RatingsService::create(IBus *bus, IRequest *req)
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

        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
    }

    // if rating already exist throw error
    QSqlQuery q;
    q.prepare("SELECT * FROM ratings WHERE absolute_uri=:absolute_uri AND user_id=:user_id");
    q.bindValue(":absolute_uri", absolute_uri);
    q.bindValue(":user_id",curUser_id);
    q.exec();

    if(q.first()){
        error.insert("error","duplicate rating");
        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
    }

    IDatabaseUpdate *createRating = m_proxyConnection->databaseUpdate();

    IDatabaseUpdateQuery *query = createRating->createUpdateQuery("ratings", IDatabaseUpdateQuery::Insert);

    query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

    query->setColumnValue("absolute_uri", absolute_uri);
    query->setColumnValue("val", value);
    query->setColumnValue("user_id", curUser_id);

    int a = createRating->execute();
    if(a){
        bus->setHttpStatus(500,"Internal server error");
        return new QVariant;
    }
    bus->setHttpStatus(201, "Created");
    return new QVariant;


}

QVariant *RatingsService::index(IBus *bus, IRequest *req)
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

        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
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

    bus->setHttpStatus(200, "OK");
    return new QVariant(ratings);
}

QVariant *RatingsService::show(IBus *bus, IRequest *req)
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

        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
    }

    QString absolute_uri = reqJson["absolute_uri"].toString();
    if(absolute_uri == ""){
        error.insert("absolute_uri","required");
        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
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

    bus->setHttpStatus(200, "OK");
    return new QVariant(ratings);
}


QVariant *RatingsService::del(IBus *bus, IRequest *req)
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

        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
    }

    QString absolute_uri = reqJson["idi"].toString();
    if(absolute_uri == ""){
        error.insert("id","required");
        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
    }


    IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();
    IDatabaseUpdateQuery *query = update->createUpdateQuery("recommendations", IDatabaseUpdateQuery::Delete);
    query->setUpdateDates(true);
    query->setWhere("id", reqJson["id"]);
    query->setWhere("user_id",curUser_id);

    if(!update->execute()){
        bus->setHttpStatus(200,"OK");
        return new QVariant;
    }
    else{
        bus->setHttpStatus(500, "Internal Server Error");
        return new QVariant;
    }

}

