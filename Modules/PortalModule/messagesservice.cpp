#include "messagesservice.h"

#include "irequest.h"
#include "idatabaseupdate.h"
#include "ibus.h"
#include "iproxyconnection.h"
#include "isession.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QMap>

MessagesService::MessagesService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

// create element
QVariant *MessagesService::create(IBus *bus, IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;
    QVariantMap error;

    bool missingValue = false;

    QString text = reqJson["text"].toString();
    if(text == ""){
        missingValue = true;
        error.insert("text","required");
    }

    QString group_id = reqJson["group_id"].toString();
    if(group_id == ""){
        missingValue = true;
        error.insert("group_id","required");
    }

    QString parent_id = reqJson["parent_id"].toString();
    if(parent_id == ""){
        missingValue = true;
        error.insert("parent_id","required");
    }
    else if(parent_id!="0"){
        QSqlQuery q;
        q.prepare("SELECT * FROM messages WHERE id=:id ");
        q.bindValue(":id", parent_id);
        q.exec();

        if(!q.first()){
            bus->setHttpStatus(400,"Bad Request");
            return new QVariant;
        }
        if(q.value(q.record().indexOf("parent_id")).toString() != "0"){
            QVariantMap qm;
            qm.insert("error","not allowed nesting level");
            bus->setHttpStatus(400,"Bad Request");
            return new QVariant(qm);
        }

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
   bool member = m_proxyConnection->callModule(request)->toBool();

   if(member){

        IDatabaseUpdate *createRecomm = m_proxyConnection->databaseUpdate();

        IDatabaseUpdateQuery *query = createRecomm->createUpdateQuery("messages", IDatabaseUpdateQuery::Insert);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated


        query->setColumnValue("text", text);
        query->setColumnValue("group_id", group_id);
        query->setColumnValue("parent_id", parent_id);
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

QVariant *MessagesService::index(IBus *bus, IRequest *req)
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
    bool member = m_proxyConnection->callModule(request)->toBool();

    if(member){
        QSqlQuery query;

        query.prepare("SELECT * FROM messages WHERE group_id = :group_id");
        query.bindValue(":group_id",group_id);

        QVariantList messages;

        while (query.next()) {
            QVariantMap message;
            message.insert("id", query.value(query.record().indexOf("id")));
            message.insert("text", query.value(query.record().indexOf("text")));
            message.insert("user_id", query.value(query.record().indexOf("user_id")));
            message.insert("parent_id", query.value(query.record().indexOf("parent_id")));

            messages.append(message);
        }

        bus->setHttpStatus(200, "OK");
        return new QVariant(messages);

    }

    bus->setHttpStatus(400,"Bad Request");
    return new QVariant;
}

QVariant *MessagesService::del(IBus *bus, IRequest *req, int id)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isAdmin");
    request->setParamater("user_id", reqJson["user_id"].toString());
    request->setParamater("group_id", reqJson["group_id"].toString());
    bool admin = m_proxyConnection->callModule(request)->toBool();

    QSqlQuery q;
    q.prepare("SELECT * FROM messages WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
    q.bindValue(":user_id",reqJson["user_id"].toString());
    q.bindValue(":group_id",reqJson["group_id"].toString());
    q.bindValue(":id",reqJson["id"]);
    q.exec();

    bool owner = q.first();

    if(admin || owner){
        IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();
        IDatabaseUpdateQuery *query = update->createUpdateQuery("messages", IDatabaseUpdateQuery::Delete);
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

