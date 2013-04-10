#include "ratingmanager.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "idatabaseselectquerywheregroup.h"
#include "isynceddatabaseupdatequery.h"

#include <QSqlRecord>
#include <QSqlQuery>
#include <QUuid>
#include <QDate>

RatingManager::RatingManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_activityManager = new ActivityManager(proxyConnection);
}

IResponse::Status RatingManager::createRating(QString userId, QString  uri, int value, QVariantMap &error)
 {

     // if rating already exist throw error
     QSqlQuery q;
     q.prepare("SELECT 1 FROM ratings WHERE absolute_uri=:absolute_uri AND user_id=:user_id");
     q.bindValue(":absolute_uri", uri);
     q.bindValue(":user_id",userId);
     q.exec();

     if(q.first()){
         error.insert("error","duplicate rating");
         return IResponse::CONFLICT;
     }

     QObject parentObject;
     IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("ratings", &parentObject);

     query->setUpdateDates(IDatabaseUpdateQuery::DateCreated);

     query->setColumnValue("absolute_uri", uri);
     query->setColumnValue("val", value);
     query->setColumnValue("user_id", userId);

     if(!query->executeQuery())
         return IResponse::INTERNAL_SERVER_ERROR;
     QString uid = query->syncedQuery()->lastUid();

     // create activity

     Activity ac;

     //username is solved inside createActivity method
     ac.activity_type = Activity::RATING;
     ac.content = uri + QString(";") + QString::number(value);
     ac.group_id = "";
     ac.object_id = uid;
     ac.user_id = userId;


     m_activityManager->createActivity(ac);


     return IResponse::CREATED;
 }

IResponse::Status RatingManager::showRating(QString uid, QVariantMap &rating, QVariantMap &error)
{
    QSqlQuery query;
    query.prepare("SELECT _id, user_id, absolute_uri, val, uid FROM ratings WHERE uid=:uid");
    query.bindValue(":uid",uid);

    if(!query.exec())
        return IResponse::INTERNAL_SERVER_ERROR;

    if (!query.first())
        return IResponse::NOT_FOUND;

    QSqlRecord row = query.record();

    rating.insert("id", row.value("_id"));
    rating.insert("user_id", row.value("user_id"));
    rating.insert("absolute_uri", row.value("absolute_uri"));
    rating.insert("value", row.value("val"));
    rating.insert("uid", row.value("uid"));

    return IResponse::OK;
}

IResponse::Status RatingManager::editRating(QString uid, uint userId, int value, QVariantMap &error)
{
    QVariantMap rating;
    IResponse::Status status = showRating(uid, rating, error);

    if (status != IResponse::OK)
        return status;

    if (rating["user_id"].toInt() != userId)
        return IResponse::UNAUTHORIEZED;

    QObject parent;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("ratings", &parent);

    query->setUpdateDates(IDatabaseUpdateQuery::DateCreated);
    query->setType(IDatabaseUpdateQuery::InsertOrUpdate);
    query->setColumnValue("val", value);

    IDatabaseSelectQueryWhereGroup *where = query->whereGroup(IDatabaseSelectQuery::And);
    where->where("uid", uid);
    where->where("user_id", userId);

    if ( query->executeQuery() )
        return IResponse::OK;
    else
        return IResponse::INTERNAL_SERVER_ERROR;
}

IResponse::Status RatingManager::deleteRating(QString uid, uint userId, QVariantMap &error)
{
    QVariantMap rating;
    IResponse::Status status = showRating(uid, rating, error);

    if (status != IResponse::OK)
        return status;

    if (rating["user_id"].toInt() != userId)
        return IResponse::UNAUTHORIEZED;

    QObject parentObject;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("ratings", &parentObject);
    query->setUpdateDates(IDatabaseUpdateQuery::DateCreated);
    query->setType(IDatabaseUpdateQuery::Delete);

    IDatabaseSelectQueryWhereGroup *where = query->whereGroup(IDatabaseSelectQuery::And);
    where->where("uid", uid);
    where->where("user_id", userId);

    if (!query->executeQuery())
        return IResponse::INTERNAL_SERVER_ERROR;
    else {
        //**** delete activity ********//
        m_activityManager->deleteActivity( rating["uid"].toString());
        return IResponse::OK;
    }
}

IResponse::Status RatingManager::showAllPageRatings(QString uri, QVariantList &ratings, QVariantMap &error)
{    
    QSqlQuery query;
    query.prepare("SELECT _id, user_id, val FROM ratings WHERE absolute_uri=:absolute_uri");
    query.bindValue(":absolute_uri",uri);

    if(!query.exec())
        return IResponse::INTERNAL_SERVER_ERROR;

    while (query.next()) {
        QSqlRecord row = query.record();
        QVariantMap rating;        
        rating.insert("id", row.value("_id"));
        rating.insert("user_id", row.value("user_id"));
        rating.insert("value", row.value("val"));
//        rating.insert("uid", query.value(query.record().indexOf("uid")));

        ratings.append(rating);
    }

    return IResponse::OK;
}

IResponse::Status RatingManager::showAllUserRatings(uint userId, QVariantList &ratings, QVariantMap &error)
{
    QSqlQuery query;
    query.prepare("SELECT _id, absolute_uri, val FROM ratings WHERE user_id=:user_id");
    query.bindValue(":user_id",userId);

    if(!query.exec())
        return IResponse::INTERNAL_SERVER_ERROR;

    while (query.next()) {
        QSqlRecord row = query.record();
        QVariantMap rating;

        rating.insert("id", row.value("_id"));
        rating.insert("absolute_uri", row.value("absolute_uri"));
        rating.insert("value", row.value("val"));
        ratings.append(rating);
    }

    return IResponse::OK;
}

IResponse::Status RatingManager::showPageStats(QString uri, uint userId, QVariantMap &stats, QVariantMap &error)
{
    QSqlQuery query;
    query.prepare("SELECT count(*) as count, AVG(val) as average FROM ratings WHERE absolute_uri=:uri");
    query.bindValue(":uri",uri);

    if(!query.exec())
        return IResponse::INTERNAL_SERVER_ERROR;

    if (!query.first())
        return IResponse::NOT_FOUND;

    QSqlRecord row = query.record();
    stats.insert("count", row.value("count"));
    stats.insert("average", row.value("average"));


    if (userId != -1) {
        QSqlQuery userQuery;
        userQuery.prepare("SELECT _id, val FROM ratings WHERE user_id=:user_id AND absolute_uri=:uri");
        userQuery.bindValue(":user_id",userId);
        userQuery.bindValue(":uri",uri);

        if(!userQuery.exec())
            return IResponse::INTERNAL_SERVER_ERROR;

        if (userQuery.first()) {
            QSqlRecord userRow = userQuery.record();
            stats.insert("id", userRow.value("_id"));
            stats.insert("val", userRow.value("val"));
       }
    }

    return IResponse::OK;
}

