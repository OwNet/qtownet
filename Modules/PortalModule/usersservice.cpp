#include "usersservice.h"

#include "irequest.h"
#include "iresponse.h"
#include "idatabaseupdate.h"
#include "ibus.h"
#include "iproxyconnection.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

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

IResponse *UsersService::show(IRequest *req, int id)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id",id);

    if (query.exec()) {

        if (query.first()) {

            QVariantMap user;
            user.insert("id", query.value(query.record().indexOf("id")));
            user.insert("first_name", query.value(query.record().indexOf("first_name")));
            user.insert("last_name", query.value(query.record().indexOf("last_name")));
            user.insert("login", query.value(query.record().indexOf("login")));
            user.insert("email", query.value(query.record().indexOf("email")));

            return req->response( QVariant(user) );
        }
    }

    return req->response(IResponse::BAD_REQUEST);
}

IResponse *UsersService::create(IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;
    QObject parent;

    QString login = reqJson["login"].toString();

    // check if user with stated exists
    QSqlQuery q_check;

    q_check.prepare("SELECT login FROM users WHERE login = :login");
    q_check.bindValue(":login",login);
    q_check.exec();

    if(q_check.first()){        
        // create answer
        QVariantMap status;
        status.insert("error", login);                
        return req->response( QVariant(status), IResponse::CONFLICT );
    }

    QString last_name = reqJson["last_name"].toString();
    QString first_name = reqJson["first_name"].toString();
    QString email = reqJson["email"].toString();
    QString password = reqJson["password"].toString();

    //creating user ID
    uint id = qHash(QString("%1-%2").arg(login).arg(QDateTime::currentDateTime().toString(Qt::ISODate)));

    IDatabaseUpdate *update = m_proxyConnection->databaseUpdate(&parent);

    IDatabaseUpdateQuery *query = update->createUpdateQuery("users", IDatabaseUpdateQuery::Insert);

    query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

    query->setColumnValue("id", id);
    query->setColumnValue("first_name", first_name);
    query->setColumnValue("last_name", last_name);
    query->setColumnValue("login", login);
    query->setColumnValue("email", email);
    query->setColumnValue("role_id", 1);
    query->setColumnValue("password", password);

    int a = update->execute();
    if(!a)
        return req->response(IResponse::CREATED);
    else
        return req->response(IResponse::BAD_REQUEST);
}

//IResponse *UsersService::edit(IRequest *, int id)
//{

//}

//IResponse *UsersService::del(IRequest *req, int id)
//{

//}
