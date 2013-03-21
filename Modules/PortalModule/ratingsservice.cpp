#include "ratingsservice.h"
#include "irouter.h"
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
    router->addRoute("/page_stats")
            ->on(IRequest::GET, ROUTE(this->showPageStats));

    router->addRoute("/page_all")
            ->on(IRequest::GET, ROUTE(this->showAllPageRatings));
}

// create element
IResponse *RatingsService::create(IRequest *req)
{
    if ( !m_proxyConnection->session()->isLoggedIn() )
           return req->response(IResponse::UNAUTHORIEZED);

    bool ok = false;
    QVariantMap error;

    uint userId = m_proxyConnection->session()->userId();

    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return req->response(IResponse::BAD_REQUEST);

    QString absolute_uri = reqJson["absolute_uri"].toString();
    if (absolute_uri == "")
        error.insert("absolute_uri","required");

    int value = reqJson["val"].toInt(&ok);
    if(!ok)
        error.insert("val","required");

    if (value<1 || value>5)
        error.insert("val","value must be between 1 and 5");

    if (!error.isEmpty())
        return req->response(QVariant(error), IResponse::BAD_REQUEST);

    IResponse::Status responseStatus;
    responseStatus = this->m_ratingManager->createRating(userId, absolute_uri, value, error);

    if (responseStatus != IResponse::CREATED)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response(responseStatus);

}


/*IResponse *RatingsService::index(IRequest *req)
{
    if ( !m_proxyConnection->session()->isLoggedIn() )
           return req->response(IResponse::UNAUTHORIEZED);

    uint userId = m_proxyConnection->session()->userId();
    QVariantList ratings;
    QVariantMap error;

    IResponse::Status responseStatus;
    responseStatus = this->m_ratingManager->showAllUserRatings(userId, ratings, error);

    if (responseStatus != IResponse::OK)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response(QVariant(ratings),responseStatus);

}*/

IResponse *RatingsService::show(IRequest *req, const QString &textId)
{
    uint id = textId.toUInt();
    if ( !m_proxyConnection->session()->isLoggedIn() )
           return req->response(IResponse::UNAUTHORIEZED);

    QVariantMap error;
    QVariantMap rating;

    IResponse::Status responseStatus = this->m_ratingManager->showRating(id, rating, error);

    if (responseStatus != IResponse::OK)
        return req->response(QVariant(error), responseStatus);
    else
        return req->response(QVariant(rating),responseStatus);
}

IResponse *RatingsService::edit(IRequest *req, const QString &textId)
{
    uint id = textId.toUInt();
    if ( !m_proxyConnection->session()->isLoggedIn() )
           return req->response(IResponse::UNAUTHORIEZED);

    bool ok;
    QVariantMap error;

    uint userId = m_proxyConnection->session()->userId();

    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return req->response(IResponse::BAD_REQUEST);

    int value = reqJson["val"].toInt(&ok);
    if(!ok)
        error.insert("val","required");

    if (!error.isEmpty())
        return req->response(QVariant(error), IResponse::BAD_REQUEST);

    IResponse::Status responseStatus = this->m_ratingManager->editRating(id, userId, value, error );

    if (responseStatus != IResponse::OK)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response(responseStatus);
}


IResponse *RatingsService::del(IRequest *req, const QString &textId)
{
    uint id = textId.toUInt();
    if ( !m_proxyConnection->session()->isLoggedIn() )
           return req->response(IResponse::UNAUTHORIEZED);

    uint userId = m_proxyConnection->session()->userId();

    QVariantMap error;

    IResponse::Status responseStatus = this->m_ratingManager->deleteRating(id, userId, error);

    if (responseStatus != IResponse::NO_CONTENT)
        return req->response(QVariant(error), responseStatus);
    else
       return req->response(responseStatus);
}

IResponse *RatingsService::showPageStats(IRequest *req)
{
    QVariantMap error;
    QVariantMap stats;

    if (!req->hasParameter("uri")) {
        error.insert("uri","required");
        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    uint userId = m_proxyConnection->session()->isLoggedIn() ? m_proxyConnection->session()->userId() : -1;

    IResponse::Status responseStatus;    
    responseStatus = this->m_ratingManager->showPageStats( req->parameterValue("uri"), userId, stats, error);

    if (responseStatus != IResponse::OK)
        return req->response(QVariant(error), responseStatus);
    else
        return req->response(QVariant(stats), responseStatus);
}

IResponse *RatingsService::showAllPageRatings(IRequest *req)
{
    if ( !m_proxyConnection->session()->isLoggedIn() )
           return req->response(IResponse::UNAUTHORIEZED);

    QVariantMap error;
    QVariantList ratings;

    if (!req->hasParameter("uri")) {
        error.insert("uri","required");
        return req->response(QVariant(error), IResponse::BAD_REQUEST);
    }

    IResponse::Status responseStatus;
    responseStatus = this->m_ratingManager->showAllPageRatings( req->parameterValue("uri"), ratings, error);

    if (responseStatus != IResponse::OK)
        return req->response(QVariant(error), responseStatus);
    else
        return req->response(QVariant(ratings), responseStatus);
}
