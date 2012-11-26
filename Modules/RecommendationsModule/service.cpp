#include "service.h"

#include "irequest.h"
#include "idatabaseupdate.h"
#include "ibus.h"
#include "iproxyconnection.h"

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

    // overit ci je userom skupiny do ktorej chce pridat recomm
    else{


        IDatabaseUpdate *createRecomm = m_proxyConnection->databaseUpdate();

        IDatabaseUpdateQuery *query = createRecomm->createUpdateQuery("recommendations", IDatabaseUpdateQuery::Insert);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated


        query->setColumnValue("title", title);
        query->setColumnValue("absolute_uri", absolute_uri);
        query->setColumnValue("group_id", group_id);
        query->setColumnValue("description", description);
        query->setColumnValue("user_id", user_id);


        int a = createGroup->execute();
        if(a){
            bus->setHttpStatus(500,"Internal server error");
            return new QVariant;
        }


    }

    bus->setHttpStatus(201, "Created");
    return new QVariant;

}

// show element
QVariant *Service::show(IBus *bus, IRequest *req)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id",req->id());

    if (query.exec()) {

        if (query.first()) {

            bus->setHttpStatus(200, "OK");

            QVariantMap user;
            user.insert("id", query.value(query.record().indexOf("id")));
            user.insert("first_name", query.value(query.record().indexOf("first_name")));
            user.insert("last_name", query.value(query.record().indexOf("last_name")));
            user.insert("login", query.value(query.record().indexOf("login")));
            user.insert("email", query.value(query.record().indexOf("email")));

            return new QVariant(user);
        }
    }

    bus->setHttpStatus(400,"Bad Request");
    return new QVariant;
}

QVariant *Service::index(IBus *bus, IRequest *)
{
    QSqlQuery query;

    if (query.exec("SELECT * FROM users")) {
        QVariantList users;

        while (query.next()) {
            QVariantMap user;
            user.insert("id", query.value(query.record().indexOf("id")));
            user.insert("first_name", query.value(query.record().indexOf("first_name")));
            user.insert("last_name", query.value(query.record().indexOf("last_name")));
            user.insert("login", query.value(query.record().indexOf("login")));
            user.insert("email", query.value(query.record().indexOf("email")));
            users.append(user);
        }

        bus->setHttpStatus(200, "OK");
        return new QVariant(users);
    }
    else
        bus->setHttpStatus(400,"Bad Request");

    return new QVariant;
}
