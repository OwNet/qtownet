#include "sessionmodule.h"

#include "modules/irequest.h"
#include "modules/ibus.h"

#include <QSqlQuery>

QVariant *SessionModule::create(IBus *bus, IRequest *req)
{
    QVariantMap response;

    // if user is already loggedIn
    if (bus->session()->isLoggedIn()) {
        bus->setHttpStatus(400, "Bad Request");
        return new QVariant;
    }

    QVariantMap reqJson = req->postBodyFromJson();

    QString login = reqJson["login"].toString();
    QString password = reqJson["password"].toString();

    QSqlQuery q;
    q.prepare("SELECT id FROM users WHERE login = :login AND password = :password");
    q.bindValue(":login", login);
    q.bindValue(":password", password);

    if (q.exec() && q.first()) {
        bus->session()->setValue("logged", q.value(0));
        bus->setHttpStatus(201,"Created");

        response.insert("user_id", q.value(0));
    } else {
        bus->setHttpStatus(400,"Bad Request");
    }
    return new QVariant(response);
}

QVariant *SessionModule::del(IBus *bus, IRequest *)
{
    QVariantMap response;

    if (!bus->session()->isLoggedIn()) {
        bus->setHttpStatus(400, "Bad Request");
        response.insert("success", false);
    }

    bus->session()->clear();
    response.insert("success", true);

    return new QVariant(response);
}

Q_EXPORT_PLUGIN2(ownet_sessionmodule, SessionModule)
