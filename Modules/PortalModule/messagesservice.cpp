#include "messagesservice.h"
#include "activitiesservice.h"

#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "isynceddatabaseupdatequery.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QMap>
#include <qmath.h>

#include "irouter.h"

#define PER_PAGE 10

MessagesService::MessagesService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_activityManager = new ActivityManager(proxyConnection);
}

void MessagesService::init(IRouter *router)
{
        router->addRoute("/allPagesCount")->on(IRequest::GET, ROUTE(allPagesCount));
}

// create element
IResponse *MessagesService::create(IRequest *req)
{
    bool ok = false;
    QVariantMap error;

    QString cur_user_id = m_proxyConnection->session()->value("logged").toString();
    if(cur_user_id == "")
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
        q.prepare("SELECT * FROM messages WHERE uid=:uid ");
        q.bindValue(":uid", parent_id);
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
   request->setParamater("user_id", cur_user_id);
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
        query->setColumnValue("user_id", cur_user_id);

        if(!query->executeQuery()){

            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }
        QString uid = query->syncedQuery()->lastUid();

        if(parent_id == "0"){
            // create activity

            Activity ac;

            //username is solved inside createActivity method
            ac.activity_type = Activity::MESSAGE;
            ac.content = message;
            ac.group_id = group_id;
            ac.object_id = uid;
            ac.user_id = cur_user_id;


            m_activityManager->createActivity(ac);
        }

        return req->response(IResponse::CREATED);

    }
    else{
       error.insert("membership in group","required");
       return req->response(QVariant(error),IResponse::UNAUTHORIEZED);
    }


}


IResponse *MessagesService::allPagesCount(IRequest *req)
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
        query.prepare("SELECT COUNT(*) AS n FROM messages WHERE group_id = :group_id AND parent_id = 0");
        query.bindValue(":group_id",group_id);
        if(query.exec()){
            query.first();
            int x =  query.value(query.record().indexOf("n")).toInt();
            QVariantMap response;
            response.insert("pages",qCeil(x/(double)PER_PAGE));
            return req->response(QVariant(response),IResponse::OK);
        }
        else
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

    }
}


IResponse *MessagesService::index(IRequest *req)
{
    QVariantMap error;

    int page;
    if(!(page= req->parameterValue("page").toInt())){
        QVariantMap error;
        error.insert("page_number","error");
        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

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

        query.prepare("SELECT * FROM messages JOIN users ON users.id = messages.user_id WHERE group_id = :group_id AND parent_id = 0 "
                      "ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (page-1)* PER_PAGE);
        query.bindValue(":group_id",group_id);

        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        QVariantList messages;

        while (query.next()) {

            QVariantMap message;
            message.insert("id", query.value(query.record().indexOf("_id")));
            message.insert("message", query.value(query.record().indexOf("message")));
            message.insert("first_name", query.value(query.record().indexOf("first_name")));
            message.insert("last_name", query.value(query.record().indexOf("last_name")));
            message.insert("gender", query.value(query.record().indexOf("gender")));
            message.insert("date_created", query.value(query.record().indexOf("date_created")));
            message.insert("user_id", query.value(query.record().indexOf("user_id")));
            message.insert("parent_id", query.value(query.record().indexOf("parent_id")));
            message.insert("uid", query.value(query.record().indexOf("uid")));
            message.insert("type", "message");

            messages.append(message);
        }

        query.prepare("SELECT * FROM messages JOIN users ON users.id = messages.user_id WHERE group_id = :group_id AND "
                      "parent_id !=0 AND parent_id IN (SELECT uid FROM messages WHERE group_id = :group_id AND parent_id = 0 "
                      "ORDER BY date_created DESC LIMIT :limit OFFSET :offset ) ORDER BY date_created DESC");
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (page-1)* PER_PAGE);
        query.bindValue(":group_id",group_id);

        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        QVariantList comments;

        while (query.next()) {

            QVariantMap comment;
            comment.insert("id", query.value(query.record().indexOf("_id")));
            comment.insert("message", query.value(query.record().indexOf("message")));
            comment.insert("first_name", query.value(query.record().indexOf("first_name")));
            comment.insert("last_name", query.value(query.record().indexOf("last_name")));
            comment.insert("gender", query.value(query.record().indexOf("gender")));
            comment.insert("user_id", query.value(query.record().indexOf("user_id")));
            comment.insert("date_created", query.value(query.record().indexOf("date_created")));
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
            QVariantList childComments;

            atSpot = false;
            j = 0;

            while(j < comments.count() &&  (!atSpot || comments.at(j).toMap().value("parent_id") == v.value("uid"))){
                if(comments.at(j).toMap().value("parent_id") == v.value("uid")){
                    childComments.append(comments.at(j).toMap());
                    atSpot = true;
                    comments.removeAt(j);
                    j--;

                }
                j++;
            }
            if(!childComments.empty())
                v.insert("comments",childComments);
            response.append(v);

        }

        return req->response(QVariant(response), IResponse::OK);
    }

    else{
       error.insert("membership in group","required");
       return req->response(QVariant(error),IResponse::UNAUTHORIEZED);
    }

}

IResponse *MessagesService::show(IRequest *req, const QString &uid)
{

    QVariantMap error;
    QString group_id;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == "")
        return req->response(IResponse::UNAUTHORIEZED);

    QSqlQuery queryGId;
    queryGId.prepare("SELECT * FROM messages WHERE uid = :uid");
    queryGId.bindValue(":uid",uid);

    if(!queryGId.exec())
        return req->response(IResponse::INTERNAL_SERVER_ERROR);

    if(queryGId.first())
        group_id = queryGId.value(queryGId.record().indexOf("group_id")).toString();
    else
        return req->response(IResponse::BAD_REQUEST);

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    QString member = m_proxyConnection->callModule(request)->body().toMap().value("member").toString();

    // overit ci je userom skupiny d
    if(member == "1"){

        QSqlQuery query;

        query.prepare("SELECT * FROM messages JOIN users ON users.id = messages.user_id WHERE messages.group_id = :group_id "
                      "AND messages.uid = :uid");
        query.bindValue(":group_id",group_id);
        query.bindValue(":uid",uid);

        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        QVariantMap message;

        if(query.first()) {

            message.insert("id", query.value(query.record().indexOf("_id")).toString());
            message.insert("message", query.value(query.record().indexOf("message")).toString());
            message.insert("first_name", query.value(query.record().indexOf("first_name")).toString());
            message.insert("last_name", query.value(query.record().indexOf("last_name")).toString());
            message.insert("date_created", query.value(query.record().indexOf("date_created")).toString());
            message.insert("user_id", query.value(query.record().indexOf("user_id")).toString());
            message.insert("parent_id", query.value(query.record().indexOf("parent_id")).toString());
            message.insert("uid", query.value(query.record().indexOf("uid")).toString());
            message.insert("type", "message");

        }


        query.prepare("SELECT * FROM messages JOIN users ON users.id = messages.user_id WHERE group_id = :group_id AND "
                      "parent_id = :parent_id  ORDER BY date_created DESC");
        query.bindValue(":group_id",group_id);
        query.bindValue(":parent_id", uid);

        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        QVariantList comments;

        while (query.next()) {
            QSqlQuery query_user;

            query_user.prepare("SELECT * FROM users WHERE id = :id");
            query_user.bindValue(":id",query.value(query.record().indexOf("user_id")));
            query_user.exec();

            QVariantMap comment;
            comment.insert("id", query.value(query.record().indexOf("_id")));
            comment.insert("message", query.value(query.record().indexOf("message")));
            comment.insert("first_name", query.value(query.record().indexOf("first_name")));
            comment.insert("last_name", query.value(query.record().indexOf("last_name")));
            comment.insert("user_id", query.value(query.record().indexOf("user_id")));
            comment.insert("date_created", query.value(query.record().indexOf("date_created")));
            comment.insert("parent_id", query.value(query.record().indexOf("parent_id")));
            comment.insert("uid", query.value(query.record().indexOf("uid")));
            comment.insert("type", "comment");

            comments.append(comment);
        }
        QVariantList response;

        if(!comments.empty())
            message.insert("comments",comments);
        response.append(message);

        return req->response(QVariant(response), IResponse::OK);
    }

    else{
       error.insert("membership in group","required");
       return req->response(QVariant(error),IResponse::UNAUTHORIEZED);
    }

}

IResponse *MessagesService::del(IRequest *req, const QString &uid)
{
    QVariantMap error;

    QString cur_user_id = m_proxyConnection->session()->value("logged").toString();
    if(cur_user_id == "")
        return req->response(IResponse::UNAUTHORIEZED);

    bool owner = false;
    bool admin = false;

    QString parent_id;

    QSqlQuery q;
    q.prepare("SELECT * FROM messages WHERE uid=:uid");
    q.bindValue(":uid",uid);
    q.exec();

    if(q.first()){
        parent_id = q.value(q.record().indexOf("parent_id")).toString();
        QString group_id;
        if(q.value(q.record().indexOf("user_id")).toString() == cur_user_id)
            owner = q.first();
        else{
            group_id = q.value(q.record().indexOf("group_id")).toString();
            QSqlQuery q2;
            q2.prepare("SELECT * FROM group_admins WHERE group_id=:group_id AND user_id=:user_id ");
            q2.bindValue(":user_id",cur_user_id);
            q2.bindValue(":group_id",group_id);
            q2.exec();

            admin = q2.first();
        }
    }

    if(owner || admin ){

        QObject parentObject;
        IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("messages", &parentObject);

        //query->setUpdateDates(true);
        query->setType(IDatabaseUpdateQuery::Delete);
        query->singleWhere("uid", uid);

        if(!query->executeQuery()){
            return req->response( IResponse::INTERNAL_SERVER_ERROR);
        }

        IDatabaseUpdateQuery *query2 = m_proxyConnection->databaseUpdateQuery("messages", &parentObject);
        query2->setUpdateDates(true);
        query2->setType(IDatabaseUpdateQuery::Delete);
        query2->singleWhere("parent_id", uid);

        if(!query2->executeQuery()){
            return req->response( IResponse::INTERNAL_SERVER_ERROR);
        }

        if(parent_id == "0")
            // delete activity
            m_activityManager->deleteActivity( uid);
        return req->response(IResponse::OK);
    }
    else{
       error.insert("membership in group","required");
       return req->response(QVariant(error),IResponse::UNAUTHORIEZED);
    }

}
