#include "sessionservice.h"

#include "irequest.h"
#include "ibus.h"
#include "isession.h"
#include "iproxyconnection.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QCryptographicHash>
#include <QString>

SessionService::SessionService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

bool SessionService::checkUserPassword(QString password, uint user_id){
    QSqlQuery query;

    query.prepare("SELECT * FROM users WHERE id = :group_id");
    query.bindValue(":group_id",user_id);
    if(!query.exec()){
        return false;
    }
    query.first();
    QString salt = query.value(query.record().indexOf("salt")).toString();
    QString user_password = query.value(query.record().indexOf("password")).toString();

    QByteArray pass_plus_salt = (password+salt).toLatin1();
    QString salted_pass(QCryptographicHash::hash(pass_plus_salt,QCryptographicHash::Sha1).toHex());

    if(salted_pass == user_password)
        return true;
    else
        return false;
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
    q.prepare("SELECT id FROM users WHERE login = :login");
    q.bindValue(":login", login);

    if(!q.exec())
        return req->response(IResponse::INTERNAL_SERVER_ERROR);

    if (q.first()) {
        QString i = q.value(q.record().indexOf("id")).toString();
        if(this->checkUserPassword(password, i.toUInt())){
            m_proxyConnection->session(&parent)->setValue("logged", q.value(0));
            response.insert("user_id", q.value(0));
        }
        else{
          return req->response(IResponse::BAD_REQUEST);
        }
    }else {
        return req->response(IResponse::BAD_REQUEST);
    }
    return req->response( QVariant(response), IResponse::CREATED);
}

IResponse *SessionService::del(IRequest * req, uint id)
{    
    QObject parent;

    if (!m_proxyConnection->session(&parent)->isLoggedIn())
        return req->response(IResponse::BAD_REQUEST);

    m_proxyConnection->session(&parent)->clear();
    return req->response(IResponse::NO_CONTENT);
}
