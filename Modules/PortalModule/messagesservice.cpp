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
    QVariantMap error;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == "")
        return req->response(IResponse::UNAUTHORIEZED);

    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok){
        error.insert("parsing_json","error");
        return req->response(QVariant(error),IResponse::BAD_REQUEST);
    }

    bool missingValue = false;

    QString message = reqJson["message"].toString();
    if(message == ""){
        missingValue = true;
        error.insert("message","required");
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
    else if(parent_id != "0"){
        QSqlQuery q;
        q.prepare("SELECT * FROM messages WHERE _id=:id ");
        q.bindValue(":id", parent_id);
        q.exec();

        if(!q.first()){
            QVariantMap qm;
            qm.insert("error","parent message does not exist");
            return req->response(QVariant(qm), IResponse::BAD_REQUEST);
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

   IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "groups", "isMember");
   request->setParamater("user_id", curUser_id);
   request->setParamater("group_id", group_id);

   QString member = m_proxyConnection->callModule(request)->body().toMap().value("member").toString();

   // overit ci je userom skupiny d
   if(member == "1")
   {

        QObject parentObject;
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("messages", &parentObject);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated


        query->setColumnValue("message", message);
        query->setColumnValue("group_id", group_id);
        query->setColumnValue("parent_id", parent_id);
        query->setColumnValue("user_id", curUser_id);

        if(!query->executeQuery()){

            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }
         return req->response(IResponse::CREATED);

    }
    else{
       error.insert("membership in group","required");
       return req->response(QVariant(error),IResponse::UNAUTHORIEZED);
    }


}

IResponse *MessagesService::index(IRequest *req)
{
    QVariantMap error;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == "")
        return req->response(IResponse::UNAUTHORIEZED);

    QString group_id = req->parameterValue("group_id");
    if(group_id == ""){
        error.insert("group_id_parameter","required");
        return req->response(QVariant(error),IResponse::BAD_REQUEST);
    }

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    QString member = m_proxyConnection->callModule(request)->body().toMap().value("member").toString();

    // overit ci je userom skupiny d
    if(member == "1"){

        QSqlQuery query;

        query.prepare("SELECT * FROM messages WHERE group_id = :group_id AND parent_id = 0 ORDER BY date_created");
        query.bindValue(":group_id",group_id);

        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        QVariantList messages;

        while (query.next()) {
            QVariantMap message;
            message.insert("id", query.value(query.record().indexOf("_id")));
            message.insert("message", query.value(query.record().indexOf("message")));
            message.insert("user_id", query.value(query.record().indexOf("user_id")));
            message.insert("parent_id", query.value(query.record().indexOf("parent_id")));
            message.insert("uid", query.value(query.record().indexOf("uid")));
            message.insert("type", "message");

            messages.append(message);
        }


        query.prepare("SELECT * FROM messages WHERE group_id = :group_id AND parent !=0 ORDER BY parent, date_created");
        query.bindValue(":group_id",group_id);

        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        QVariantList comments;

        while (query.next()) {
            QVariantMap comment;
            comment.insert("id", query.value(query.record().indexOf("_id")));
            comment.insert("message", query.value(query.record().indexOf("message")));
            comment.insert("user_id", query.value(query.record().indexOf("user_id")));
            comment.insert("parent_id", query.value(query.record().indexOf("parent_id")));
            comment.insert("uid", query.value(query.record().indexOf("uid")));
            comment.insert("type", "comment");


            comments.append(comment);
        }
        QVariantList response;
        int n = messages.count();
        bool atSpot = false;
        int j=0;
        for(int i = 0; i< n; i++)
        {
            QVariantMap v = messages.at(i).toMap();
            response.append(v);
            atSpot = false;
            j = 0;
            while(!atSpot || comments.at(j).toMap().value("parent_id") == v.value("uid")){
                if(comments.at(j).toMap().value("parent_id") == v.value("uid")){
                    response.append(comments.at(j));
                    atSpot = true;
                    comments.removeAt(j);
                }
                j++;
            }


        }

        return req->response(QVariant(response), IResponse::OK);
    }

    else{
       error.insert("membership in group","required");
       return req->response(QVariant(error),IResponse::UNAUTHORIEZED);
    }

}

IResponse *MessagesService::del(IRequest *req, uint id)
{
    QVariantMap error;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == "")
        return req->response(IResponse::UNAUTHORIEZED);

    QString group_id = req->parameterValue("group_id");
    if(group_id == ""){
        error.insert("group_id_parameter","required");
        return req->response(QVariant(error),IResponse::BAD_REQUEST);
    }

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "groups", "isAdmin");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    QString admin = m_proxyConnection->callModule(request)->body().toMap().value("admin").toString();

    bool owner;
    // overit ci je adminom skupiny d
    if(admin == "1")
        owner = true;
    else{

        QSqlQuery q;
        q.prepare("SELECT * FROM messages WHERE id=:id AND user_id=:user_id AND group_id =:group_id");
        q.bindValue(":user_id",curUser_id);
        q.bindValue(":group_id",group_id);
        q.bindValue(":id",id);
        q.exec();

        owner = q.first();
    }

    if(owner){

        QObject parentObject;
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("messages", &parentObject);

        query->setUpdateDates(true);
        query->setType(IDatabaseUpdateQuery::Delete);
        query->singleWhere("_id", id);

        if(!query->executeQuery()){
            return req->response( IResponse::INTERNAL_SERVER_ERROR);
        }

        query->setUpdateDates(true);
        query->setType(IDatabaseUpdateQuery::Delete);
        query->singleWhere("parent_id", id);

        if(!query->executeQuery()){
            return req->response( IResponse::INTERNAL_SERVER_ERROR);
        }

        return req->response(IResponse::OK);
    }
    else{
       error.insert("membership in group","required");
       return req->response(QVariant(error),IResponse::UNAUTHORIEZED);
    }

}

