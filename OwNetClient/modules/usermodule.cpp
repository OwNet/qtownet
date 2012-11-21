#include "usermodule.h"

#include "database/databaseupdate.h"
#include "helpers/qjson/parser.h"
#include "helpers/qjson/serializer.h"
#include "irequest.h"
#include "ibus.h"

#include <QSqlQuery>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QVariant>
#include <QDateTime>
#include <QHash>
#include <QSqlRecord>

UserModule::UserModule()
{
}

// create element
QByteArray* UserModule::create(IBus *bus, IRequest *req)
{
    QJson::Parser *p = new QJson::Parser();
    QVariantMap reqJson;

    bool ok;
    reqJson = p->parse(req->requestBody(), &ok).toMap();

    if(ok){

        QString login = reqJson["login"].toString();

        // check if user with stated exists
        QSqlQuery q_check;

        q_check.prepare("SELECT login FROM users WHERE login = :login");
        q_check.bindValue(":login",login);
        q_check.exec();

        if(q_check.first()){
            bus->setHttpStatus(409, "Conflict");
            // create answer
            QVariantMap status;
            status.insert("error", login);
            QJson::Serializer serializer;
            QByteArray *json = new QByteArray(serializer.serialize(status));

            return json;
        }


        QString last_name = reqJson["last_name"].toString();
        QString first_name = reqJson["first_name"].toString();
        QString email = reqJson["email"].toString();
        QString password = reqJson["password"].toString();

        //creating user ID
        uint id = qHash(QString("%1-%2").arg(login).arg(QDateTime::currentDateTime().toString(Qt::ISODate)));

        DatabaseUpdate update;

        DatabaseUpdateQuery *query = update.createUpdateQuery("users", DatabaseUpdateQuery::Insert);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        query->setColumnValue("id", id);
        query->setColumnValue("first_name", first_name);
        query->setColumnValue("last_name", last_name);
        query->setColumnValue("login", login);
        query->setColumnValue("email", email);
        query->setColumnValue("role_id", 1);
        query->setColumnValue("password", password);

        int a = update.execute();
        if(!a)
            bus->setHttpStatus(201, "Created");
        else
            bus->setHttpStatus(400,"Bad Request");


        // create answer
        return new QByteArray();

    }

    bus->setHttpStatus(400, "Bad Request");
    return new QByteArray();


}

// show element
QByteArray* UserModule::show( IBus *bus, IRequest *req)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id",req->id());

    if( query.exec()){

        QJson::Serializer serializer;
        QVariantMap user;

        if(query.first()){

            bus->setHttpStatus(200, "OK");

            user.insert("id", query.value(query.record().indexOf("id")));
            user.insert("first_name", query.value(query.record().indexOf("first_name")));
            user.insert("last_name", query.value(query.record().indexOf("last_name")));
            user.insert("login", query.value(query.record().indexOf("login")));
            user.insert("email", query.value(query.record().indexOf("email")));

            QByteArray *json = new QByteArray(serializer.serialize(user));
            return json;
        }
    }

    bus->setHttpStatus(400,"Bad Request");
    return new QByteArray();
}

QByteArray* UserModule::index(IBus *bus,  IRequest *)
{

    QSqlQuery query;

    if (query.exec("SELECT * FROM users")) {

        QJson::Serializer serializer;
        QVariantList users;

        while(query.next()) {
            QVariantMap user;
            user.insert("id", query.value(query.record().indexOf("id")));
            user.insert("first_name", query.value(query.record().indexOf("first_name")));
            user.insert("last_name", query.value(query.record().indexOf("last_name")));
            user.insert("login", query.value(query.record().indexOf("login")));
            user.insert("email", query.value(query.record().indexOf("email")));
            users.append(user);
        }

        bus->setHttpStatus(200, "OK");
        QByteArray *json = new QByteArray(serializer.serialize(users));
        return json;
    }
    else
        bus->setHttpStatus(400,"Bad Request");

    return new QByteArray();
}

/*QByteArray* UserModule::registerUser(IBus *bus, QByteArray data)
{
    QSqlQuery q;

    QSqlQuery query;

    query.prepare("INSERT INTO employee (id, name, salary) "
                  "VALUES (:id, :name, :salary)");
    query.bindValue(":id", 1001);
    query.bindValue(":name", "Thad Beaumont");
    query.bindValue(":salary", 65000);
    query.exec();

    if(q.exec())
        bus->setHttpStatus(200, "OK");
    else
        bus->setHttpStatus(400,"Bad Request");


}

QByteArray* UserModule::getAllUsers(IBus *bus, QByteArray data)
{
}*/
