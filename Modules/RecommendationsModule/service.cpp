#include "service.h"

#include "irequest.h"
#include "idatabaseupdate.h"
#include "ibus.h"
#include "iproxyconnection.h"
#include "isession.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

Service::Service(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

// create element
QVariant *Service::create(IBus *bus, IRequest *req)
{
    QVariantMap reqJson = req->postBodyFromJson();
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

        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
    }

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);
    bool member = bus->callModule(request)->toBool();

    // overit ci je userom skupiny do ktorej chce pridat recomm
    if(member){


        IDatabaseUpdate *createRecomm = m_proxyConnection->databaseUpdate();

        IDatabaseUpdateQuery *query = createRecomm->createUpdateQuery("recommendations", IDatabaseUpdateQuery::Insert);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated


        query->setColumnValue("title", title);
        query->setColumnValue("absolute_uri", absolute_uri);
        query->setColumnValue("group_id", group_id);
        query->setColumnValue("description", description);
        query->setColumnValue("user_id", curUser_id);


        int a = createRecomm->execute();
        if(a){
            bus->setHttpStatus(500,"Internal server error");
            return new QVariant;
        }
        bus->setHttpStatus(201, "Created");
        return new QVariant;

    }
    else{
        bus->setHttpStatus(400, "Bad Request");
        return new QVariant;
    }


}

QVariant *Service::index(IBus *bus, IRequest *req)
{

    QVariantMap reqJson = req->postBodyFromJson();


    QString group_id = reqJson["group_id"].toString();
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);
    bool member = bus->callModule(request)->toBool();

    // overit ci je userom skupiny do ktorej chce pridat recomm
    if(member){
        QSqlQuery query;

        query.prepare("SELECT * FROM recommendations WHERE user_id = :user_id AND group_id = :group_id");
        query.bindValue(":user_id",curUser_id);
        query.bindValue(":group_id",group_id);

        QVariantList recomms;

        while (query.next()) {
            QVariantMap recomm;
            recomm.insert("id", query.value(query.record().indexOf("id")));
            recomm.insert("title", query.value(query.record().indexOf("title")));
            recomm.insert("description", query.value(query.record().indexOf("description")));
            recomm.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));

            recomms.append(recomm);
        }

        bus->setHttpStatus(200, "OK");
        return new QVariant(recomms);

    }

    bus->setHttpStatus(400,"Bad Request");
    return new QVariant;
}

QVariant *Service::edit(IBus *bus, IRequest *req)
{

    QVariantMap reqJson = req->postBodyFromJson();

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isAdmin");
    request->setParamater("user_id", reqJson["user_id"].toString());
    request->setParamater("group_id", reqJson["group_id"].toString());
    bool admin = bus->callModule(request)->toBool();

    QSqlQuery q;
    q.prepare("SELECT * FROM recommendations WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",reqJson["user_id"].toString());
    q.bindValue(":group_id",reqJson["group_id"].toString());
    q.bindValue(":id",reqJson["id"]);
    q.exec();

    bool owner = q.first();

    if(admin || owner){
        IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();
        IDatabaseUpdateQuery *query = update->createUpdateQuery("recommendations", IDatabaseUpdateQuery::Update);
        query->setUpdateDates(true);
        query->setWhere("id", reqJson["id"]);

        if(reqJson["title"] != "")
            query->setColumnValue("title", reqJson["title"].toString());

        if(reqJson["description"] != "")
            query->setColumnValue("description", reqJson["description"].toString());

        if(!update->execute()){
            bus->setHttpStatus(200,"OK");
            return new QVariant;
        }
        else{
            bus->setHttpStatus(500, "Internal Server Error");
            return new QVariant;
        }
    }
    else{
        bus->setHttpStatus(400, "Bad Request");
        return new QVariant;
    }

}

QVariant *Service::del(IBus *bus, IRequest *req)
{

    QVariantMap reqJson = req->postBodyFromJson();

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isAdmin");
    request->setParamater("user_id", reqJson["user_id"].toString());
    request->setParamater("group_id", reqJson["group_id"].toString());
    bool admin = bus->callModule(request)->toBool();

    QSqlQuery q;
    q.prepare("SELECT * FROM recommendations WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",reqJson["user_id"].toString());
    q.bindValue(":group_id",reqJson["group_id"].toString());
    q.bindValue(":id",reqJson["id"]);
    q.exec();

    bool owner = q.first();

    if(admin || owner){
        IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();
        IDatabaseUpdateQuery *query = update->createUpdateQuery("recommendations", IDatabaseUpdateQuery::Delete);
        query->setUpdateDates(true);
        query->setWhere("id", reqJson["id"]);

        if(!update->execute()){
            bus->setHttpStatus(200,"OK");
            return new QVariant;
        }
        else{
            bus->setHttpStatus(500, "Internal Server Error");
            return new QVariant;
        }
    }
    else{
        bus->setHttpStatus(400, "Bad Request");
        return new QVariant;
    }

}

