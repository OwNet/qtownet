#include "sessionmodule.h"
#include <QSqlQuery>
#include "ibus.h"
#include "helpers/qjson/parser.h"

SessionModule::SessionModule(QObject *parent) :
    IModule(parent)
{
    setUrl("session");
}

bool SessionModule::isLoggedIn()
{
    return this->m_sessionData.contains("logged");
}

QByteArray* SessionModule::create(IBus *bus, ProxyRequest *req)
{
    QJson::Parser *p = new QJson::Parser();
    QVariantMap reqJson;

    bool ok;

    // if user is already loggedIn
    if(isLoggedIn()){
        bus->setHttpStatus(400,"Bad Request");
        return new QByteArray();
    }

    reqJson = p->parse(req->requestBody(), &ok).toMap();

    if(ok){

        QString login = reqJson["login"].toString();
        QString password = reqJson["password"].toString();

        QSqlQuery q;
        q.prepare("SELECT id FROM users WHERE login = :login AND password = :password");
        q.bindValue(":login",login);
        q.bindValue(":password",password);

        if(q.exec() && q.first())
        {
            this->m_sessionData.insert("logged",q.value(0));
            bus->setHttpStatus(201,"Created");
            return new QByteArray();
        }

    }

    bus->setHttpStatus(400,"Bad Request");
    return new QByteArray();
}

QByteArray* SessionModule::del(IBus *bus, ProxyRequest *req)
{

    if(!isLoggedIn()){
        bus->setHttpStatus(400,"Bad Request");
        return new QByteArray();
    }

    this->m_sessionData.clear();
    bus->setHttpStatus(204,"No Content");
    return new QByteArray();
}
