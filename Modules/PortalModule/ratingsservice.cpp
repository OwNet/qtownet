#include "ratingsservice.h"
#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "idatabaseselectquerywheregroup.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

RatingsService::RatingsService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_ratingManager = new RatingManager(proxyConnection);
}

void RatingsService::init(IRouter *router)
{

}

// create element
IResponse *RatingsService::create(IRequest *req)
{

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == ""){
        return req->response(IResponse::UNAUTHORIEZED);
    }

    QVariantMap error;
    IResponse::Status responseStatus;

    responseStatus= this->m_ratingManager->createRating(req, curUser_id, error);

    if(!error.empty() || responseStatus == IResponse::INTERNAL_SERVER_ERROR)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response(responseStatus);

}

/*IResponse *RatingsService::index(IRequest *req)
{
    bool logged = false;
    QVariantMap error;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == ""){
        logged = true;
        error.insert("error","not logged in");

    }
    // if not logged in
    if(logged){

        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM ratings");
    query.exec();

    QVariantList ratings;

    while (query.next()) {
        QVariantMap rating;
        rating.insert("id", query.value(query.record().indexOf("id")));
        rating.insert("user_id", query.value(query.record().indexOf("user_id")));
        rating.insert("value", query.value(query.record().indexOf("value")));
        rating.insert("absolute_uri", query.value(query.record().indexOf("absolute_uri")));

        ratings.append(rating);
    }

    return req->response(QVariant(ratings), IResponse::OK);
}*/

IResponse *RatingsService::show(IRequest *req)
{

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(curUser_id == ""){
        return req->response(IResponse::UNAUTHORIEZED);
    }

    QVariantList ratings;
    QVariantMap error;

    IResponse::Status responseStatus = this->m_ratingManager->showRating(req, ratings,error);

    if(!error.empty() || responseStatus == IResponse::INTERNAL_SERVER_ERROR)
    {
        return req->response(QVariant(error), responseStatus);
    }
    else
    {
        return req->response(QVariant(ratings),responseStatus);
    }

}


IResponse *RatingsService::del(IRequest *req)
{

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    if(curUser_id == ""){

        return req->response(IResponse::UNAUTHORIEZED);

    }

     QVariantMap error;

    IResponse::Status responseStatus = this->m_ratingManager->deleteRating(req, curUser_id, error);

    if(!error.empty() || responseStatus == IResponse::INTERNAL_SERVER_ERROR)
    {
        return req->response(QVariant(error), responseStatus);
    }
    else
    {
        return req->response(responseStatus);
    }


}

