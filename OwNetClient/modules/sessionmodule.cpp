#include "sessionmodule.h"
#include <QSqlQuery>
#include "ibus.h"
#include "helpers/qjson/parser.h"
#include "helpers/qjson/serializer.h"

SessionModule::SessionModule()
{
}

bool SessionModule::isLoggedIn()
{
    return this->m_sessionData.contains("logged");
}

QByteArray *SessionModule::create(IBus *bus, IRequest *req)
{
    QJson::Parser *p = new QJson::Parser();
    QVariantMap reqJson;
     QJson::Serializer serializer;

    bool ok;

    // if user is already loggedIn
    if (isLoggedIn()) {
        bus->setHttpStatus(400,"Bad Request");
        return new QByteArray();
    }

    reqJson = p->parse(req->requestBody(), &ok).toMap();

    if (ok) {
        QString login = reqJson["login"].toString();
        QString password = reqJson["password"].toString();

        QSqlQuery q;
        q.prepare("SELECT id FROM users WHERE login = :login AND password = :password");
        q.bindValue(":login",login);
        q.bindValue(":password",password);

        if (q.exec() && q.first()) {
            m_sessionData.insert("logged",q.value(0));
            bus->setHttpStatus(201,"Created");

            QVariantMap user;
            user.insert("user_id", q.value(0));
            QByteArray *json = new QByteArray(serializer.serialize(user));

            return json;
        }

    }

    bus->setHttpStatus(400,"Bad Request");
    return new QByteArray();
}

QByteArray *SessionModule::del(IBus *bus, IRequest *req)
{

    if (!isLoggedIn()) {
        bus->setHttpStatus(400, "Bad Request");
        return new QByteArray();
    }

    m_sessionData.clear();
    bus->setHttpStatus(204, "No Content");
    return new QByteArray();
}
