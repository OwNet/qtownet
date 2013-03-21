#include "usersservice.h"

#include "irequest.h"
#include "iresponse.h"
#include "iproxyconnection.h"

#include "portalhelper.h"
#include "isession.h"
#include "idatabaseupdatequery.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QDebug>

UsersService::UsersService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

IResponse *UsersService::index(IRequest * req)
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

        return req->response(QVariant(users));
    }
    else
        return req->response(IResponse::BAD_REQUEST);
}

IResponse *UsersService::show(IRequest *req, const QString &textId)
{
    uint id = textId.toUInt();
    QSqlQuery query;

    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id",id);
    query.exec();

    if ( query.first() ) {
        QVariantMap user;
        QSqlRecord row = query.record();

        user.insert("id", row.value("id"));
        user.insert("first_name", row.value("first_name"));
        user.insert("last_name", row.value("last_name"));
        user.insert("login", row.value("login"));
        user.insert("email", row.value("email"));

        return req->response( QVariant(user) );
    }

    return req->response(IResponse::NOT_FOUND);
}

IResponse *UsersService::create(IRequest *req)
{

    QString salt = "";

    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();

    if (!ok)
        return req->response( IResponse::BAD_REQUEST );

    QString login = reqJson["login"].toString();

    // check if user with stated exists
    QSqlQuery q_check;

    q_check.prepare("SELECT login FROM users WHERE login = :login");
    q_check.bindValue(":login",login);
    q_check.exec();

    if(q_check.first()) {
        // create answer
        QVariantMap status;
        status.insert("error", login);                
        return req->response( QVariant(status), IResponse::CONFLICT );
    }

    QString last_name = reqJson["last_name"].toString();
    QString first_name = reqJson["first_name"].toString();
    QString email = reqJson["email"].toString();

    QString password = reqJson["password"].toString();

    //ad salt
    PortalHelper::addSalt(&password,&salt);

    //creating user ID
    uint id = qHash(QString("%1-%2").arg(login).arg(QDateTime::currentDateTime().toString(Qt::ISODate)));

    QObject parent;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("users", &parent);

    query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated
    query->setColumnValue("id", id);
    query->setColumnValue("first_name", first_name);
    query->setColumnValue("last_name", last_name);
    query->setColumnValue("login", login);
    query->setColumnValue("email", email);
    query->setColumnValue("role_id", 1);
    query->setColumnValue("password", password);
    query->setColumnValue("salt", salt);

    if ( !query->executeQuery() )
       return req->response(IResponse::INTERNAL_SERVER_ERROR);

    /* auto assign into general group */
    IDatabaseUpdateQuery *query2 = m_proxyConnection->databaseUpdateQuery("group_users", &parent);

    query2->setUpdateDates(true);
    query2->setColumnValue("group_id", "0");
    query2->setColumnValue("status", "1");
    query2->setColumnValue("user_id", id);

    if ( !query2->executeQuery() )
       return req->response(IResponse::INTERNAL_SERVER_ERROR);

    return req->response(IResponse::CREATED);
}

IResponse *UsersService::edit(IRequest *req, const QString &textId)
{
    uint id = textId.toUInt();
    ISession* sess = m_proxyConnection->session();
    if ( !sess->isLoggedIn()  ||  (sess->value("logged").toUInt() != id) )
        return req->response(IResponse::UNAUTHORIEZED);


    QObject parent;

    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("users", &parent);

    bool ok = false;

    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return req->response(IResponse::INTERNAL_SERVER_ERROR);


    QString first_name = reqJson["first_name"].toString();
    if(first_name != "")
        query->setColumnValue("first_name", first_name);

    QString last_name = reqJson["last_name"].toString();
    if(last_name != "")
        query->setColumnValue("last_name", last_name);

    QString login = reqJson["login"].toString();
    if(login != "")
        query->setColumnValue("login", login);

    QString email = reqJson["email"].toString();
    if(email != "")
        query->setColumnValue("email", email);

    query->singleWhere("id", id);

    QString password = reqJson["password"].toString();
    if(password != ""){
        query->setColumnValue("password", password);
        QSqlQuery q;
        q.prepare("SELECT salt FROM groups WHERE id = :id");
        q.bindValue(":id",id);
        q.exec();

        QString salt =  q.value(q.record().indexOf("salt")).toString();
        PortalHelper::addSalt(&password,&salt);
        query->setColumnValue("password", password);

    }
    if ( query->executeQuery() )
        return req->response(IResponse::OK);
    else
        return req->response(IResponse::INTERNAL_SERVER_ERROR);
}

IResponse *UsersService::del(IRequest *req, const QString &textId)
{
    uint id = textId.toUInt();
    ISession* sess = m_proxyConnection->session();
    if ( !sess->isLoggedIn()  ||  (sess->value("logged").toUInt() != id) )
        return req->response(IResponse::UNAUTHORIEZED);

    QObject parent;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("users", &parent);
    query->setType(IDatabaseUpdateQuery::Delete);
    query->singleWhere("id", id);

    IDatabaseUpdateQuery *del_q = m_proxyConnection->databaseUpdateQuery("group_admins", &parent);
    del_q->setType(IDatabaseUpdateQuery::Delete);
    del_q->setUpdateDates(true);
    del_q->singleWhere("user_id", id);
    del_q->executeQuery();

    IDatabaseUpdateQuery *del_q2 = m_proxyConnection->databaseUpdateQuery("group_users", &parent);
    del_q2->setType(IDatabaseUpdateQuery::Delete);
    del_q2->setUpdateDates(true);
    del_q2->singleWhere("user_id", id);
    del_q2->executeQuery();

   return req->response(IResponse::NO_CONTENT);

}
