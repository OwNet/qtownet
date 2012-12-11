#include "sessionservice.h"

#include "irequest.h"
#include "isession.h"
#include "iproxyconnection.h"

#include <QSqlQuery>

SessionService::SessionService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

IResponse *SessionService::create(IRequest *req)
{
    QVariantMap response;
    QObject parent;

    // if user is already loggedIn
    if (m_proxyConnection->session(&parent)->isLoggedIn()) {        
        return req->response(IResponse::BAD_REQUEST);
    }

    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QString login = reqJson["login"].toString();
    QString password = reqJson["password"].toString();

    QSqlQuery q;
    q.prepare("SELECT id FROM users WHERE login = :login AND password = :password");
    q.bindValue(":login", login);
    q.bindValue(":password", password);

    if (q.exec() && q.first()) {
        m_proxyConnection->session(&parent)->setValue("logged", q.value(0));
        response.insert("user_id", q.value(0));
    } else {
        return req->response(IResponse::BAD_REQUEST);
    }
    return req->response( QVariant(response), IResponse::CREATED);
}

IResponse *SessionService::del(IRequest * req, int id)
{    
    QObject parent;

    if (!m_proxyConnection->session(&parent)->isLoggedIn())
        return req->response(IResponse::BAD_REQUEST);

    m_proxyConnection->session(&parent)->clear();
    return req->response(IResponse::NO_CONTENT);
}
