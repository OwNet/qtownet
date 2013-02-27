#include "messagesservice.h"

#include "irequest.h"
#include "idatabaseupdate.h"
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

void MessagesService::init(IRouter *router)
{

}

// create element
IResponse *MessagesService::create(IRequest *req)
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
            return req->response(IResponse::BAD_REQUEST);
        }
        if(q.value(q.record().indexOf("parent_id")).toString() != "0"){
            QVariantMap qm;
            qm.insert("error","not allowed nesting level");

            return req->response(QVariant(qm), IResponse::BAD_REQUEST);
        }

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

   if(member){

        QObject parentObject;
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("messages", &parentObject);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated


        query->setColumnValue("text", text);
        query->setColumnValue("group_id", group_id);
        query->setColumnValue("parent_id", parent_id);
        query->setColumnValue("user_id", curUser_id);

        if(!query->executeQuery()){

            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }
         return req->response(IResponse::CREATED);

    }
    else{
       return req->response(IResponse::BAD_REQUEST);
    }


}

IResponse *MessagesService::index(IRequest *req)
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

        return req->response(QVariant(messages), IResponse::OK);
    }

    return req->response(IResponse::BAD_REQUEST);
}

IResponse *MessagesService::del(IRequest *req, int id)
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
    q.prepare("SELECT * FROM messages WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
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

            return req->response( IResponse::INTERNAL_SERVER_ERROR);
        }
    }
    else{
        return req->response( IResponse::BAD_REQUEST);
    }

}

