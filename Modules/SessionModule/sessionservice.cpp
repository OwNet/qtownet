#include "sessionservice.h"

#include "irequest.h"
#include "ibus.h"
#include "isession.h"
#include "iproxyconnection.h"

#include <QSqlQuery>

SessionService::SessionService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

QVariant *SessionService::create(IBus *bus, IRequest *req)
{
    QVariantMap response;
    QObject parent;

    // if user is already loggedIn
    if (m_proxyConnection->session(&parent)->isLoggedIn()) {
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
        m_proxyConnection->session(&parent)->setValue("logged", q.value(0));
        bus->setHttpStatus(201,"Created");

        response.insert("user_id", q.value(0));
    } else {
        bus->setHttpStatus(400,"Bad Request");
    }
    return new QVariant(response);
}

QVariant *SessionService::del(IBus *bus, IRequest *)
{
    QVariantMap response;
    QObject parent;

    if (!m_proxyConnection->session(&parent)->isLoggedIn()) {
        bus->setHttpStatus(400, "Bad Request");
        response.insert("success", false);
    }

    m_proxyConnection->session(&parent)->clear();
    response.insert("success", true);

    return new QVariant(response);
}
