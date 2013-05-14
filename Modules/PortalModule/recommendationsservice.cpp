#include "recommendationsservice.h"

#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "isession.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

RecommendationsService::RecommendationsService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
     m_recommendationManager = new RecommendationManager(proxyConnection);
}

void RecommendationsService::init(IRouter *router)
{

}

// create element
IResponse *RecommendationsService::create(IRequest *req)
{
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(curUser_id == "")
    {
        return req->response(IResponse::UNAUTHORIZED);
    }

    QVariantMap error;
    IResponse::Status responseStatus;

    responseStatus= this->m_recommendationManager->createRecomm(req, curUser_id, error);

    if(!error.empty() || responseStatus == IResponse::INTERNAL_SERVER_ERROR)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response(responseStatus);

}

/*IResponse *RecommendationsService::index(IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QString group_id = reqJson["group_id"].toString();
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "groups", "isMember");
    request->setParamater("user_id", curUser_id);
    request->setParamater("group_id", group_id);

    bool member = m_proxyConnection->callModule(request)->body().toBool();

    // overit ci je userom skupiny do ktorej chce pridat recomm
    if(member){
        QSqlQuery query;

        query.prepare("SELECT * FROM recommendations WHERE group_id = :group_id");
        query.bindValue(":group_id",group_id);
        query.exec();

        QVariantList recomms;

        while (query.next()) {
            QVariantMap recomm;
            recomm.insert("id", query.value(query.record().indexOf("id")));
            recomm.insert("title", query.value(query.record().indexOf("title")));
            recomm.insert("description", query.value(query.record().indexOf("description")));
            recomm.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));

            recomms.append(recomm);
        }

        return req->response(QVariant(recomms), IResponse::OK);

    }

    return req->response(IResponse::BAD_REQUEST);
}*/

IResponse *RecommendationsService::show(IRequest *req, const QString &uid)
{

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(curUser_id == "")
    {
        return req->response(IResponse::UNAUTHORIZED);
    }

    QVariantMap error;
    QVariantMap recommendation;
    IResponse::Status responseStatus;

    responseStatus= this->m_recommendationManager->showRecomm(req, uid ,curUser_id, recommendation, error);

    if(!error.empty() || responseStatus == IResponse::INTERNAL_SERVER_ERROR)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response( QVariant(recommendation), responseStatus);

}

IResponse *RecommendationsService::edit(IRequest *req, const QString &uid)
{

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(curUser_id == "")
    {
        return req->response(IResponse::UNAUTHORIZED);
    }

    QVariantMap error;
    IResponse::Status responseStatus;

    responseStatus= this->m_recommendationManager->editRecomm(req, uid, curUser_id, error);

    if(!error.empty() || responseStatus == IResponse::INTERNAL_SERVER_ERROR)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response(responseStatus);

}

IResponse *RecommendationsService::del(IRequest *req, const QString &uid)
{
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(curUser_id == "")
    {
        return req->response(IResponse::UNAUTHORIZED);
    }

    QVariantMap error;
    IResponse::Status responseStatus;

    responseStatus= this->m_recommendationManager->deleteRecomm(req, uid,curUser_id, error);

    if(!error.empty() || responseStatus == IResponse::INTERNAL_SERVER_ERROR)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response(responseStatus);


}


