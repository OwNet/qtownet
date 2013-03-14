#include "activitymanager.h"
#include <QSqlQuery>
#include <QSqlRecord>

#include "idatabaseupdatequery.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "qmath.h"

#define PER_PAGE 10

ActivityManager::ActivityManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

bool ActivityManager::createActivity(Activity &ac)
{

    QObject parent;
    ISession* sess = m_proxyConnection->session(&parent);
    ac.user_name = sess->value("logged").toString();

    QObject parentObject;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("activities", &parentObject);

    query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

    query->setColumnValue("user_name", ac.user_name);
    query->setColumnValue("content", ac.content);
    query->setColumnValue("type", ac.activity_type);
    query->setColumnValue("group_id", ac.group_id);
    query->setColumnValue("user_id", ac.user_id);
    query->setColumnValue("object_id", ac.object_id);

    if(!query->executeQuery()){
        return false;
    }
    else
    {
        return true;
    }
}

int ActivityManager::pagesCount(IRequest *req)
{

    //TODO add some validations + error response trought parameter

    QString type = req->parameterValue("type");

    if(type == "")
    {
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) AS n FROM activities");
        if(query.exec())
            query.first();
            int x =  query.value(query.record().indexOf("n")).toInt();
            return qCeil(x/(double)PER_PAGE);

    }
    else{
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) AS n FROM activities WHERE type=:type");
        query.bindValue(":type",type);
        if(query.exec()){
            query.first();
            int x =  query.value(query.record().indexOf("n")).toInt();
            return qCeil(x/(double)PER_PAGE);
        }
        return 0;

    }
}


QVariantList ActivityManager::getActivities(bool *ok, QVariantMap &error, IRequest *req)
{


    QString type = req->parameterValue("type");

    int intPage;
    if(!(intPage= req->parameterValue("page").toInt())){
        error.insert("page_number","error");
    }


    if(type == "")
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM activities ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (intPage-1)* PER_PAGE);
        if(!query.exec())
            *ok = false;

        QVariantList activities;

        while (query.next()) {
            QVariantMap activity;
            activity.insert("id", query.value(query.record().indexOf("id")));
            activity.insert("user_name", query.value(query.record().indexOf("user_name")));
            activity.insert("content", query.value(query.record().indexOf("content")));
            activity.insert("type", query.value(query.record().indexOf("type")));
            activity.insert("date_created", query.value(query.record().indexOf("date_created")));

            activities.append(activity);
        }

        *ok = true;
        return activities;
    }
    else
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM activities WHERE type = :type ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (intPage-1)* PER_PAGE);
        query.bindValue(":type",type);
        if(!query.exec())
            *ok = false;

        QVariantList activities;

        while (query.next()) {
            QVariantMap activity;
            activity.insert("id", query.value(query.record().indexOf("id")));
            activity.insert("user_name", query.value(query.record().indexOf("user_name")));
            activity.insert("content", query.value(query.record().indexOf("content")));
            activity.insert("type", query.value(query.record().indexOf("type")));
            activity.insert("date_created", query.value(query.record().indexOf("date_created")));

            activities.append(activity);
        }

        *ok = true;
        return activities;

    }

}


int ActivityManager::myPagesCount(IRequest *req)
{

    QString curUserId = m_proxyConnection->session()->value("logged").toString();

    //TODO add some validations + error response trought parameter

    QString type = req->parameterValue("type");

    if(type == "")
    {
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) AS n FROM activities WHERE user_id = :user_id");
        query.bindValue(":user_id",curUserId);
        if(query.exec())
            query.first();
            int x =  query.value(query.record().indexOf("n")).toInt();
            return qCeil((double)x/(double)PER_PAGE);

    }
    else{
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) AS n FROM activities WHERE type=:type AND user_id=:user_id");
        query.bindValue(":type",type);
        query.bindValue(":user_id",curUserId);
        if(query.exec()){
            query.first();
            int x =  query.value(query.record().indexOf("n")).toInt();
            return qCeil(x/(double)PER_PAGE);
        }
        return 0;

    }
}




QVariantList ActivityManager::getMyActivities(bool *ok, QVariantMap &error, IRequest *req)
{
    QString curUserId = m_proxyConnection->session()->value("logged").toString();

    QString type = req->parameterValue("type");

    int intPage;
    if(!(intPage= req->parameterValue("page").toInt())){
        error.insert("page_number","error");
    }


    if(type == "")
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM activities WHERE user_id = :user_id ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":user_id",curUserId);
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (intPage-1)* PER_PAGE);
        if(!query.exec())
            *ok = false;

        QVariantList activities;

        while (query.next()) {
            QVariantMap activity;
            activity.insert("id", query.value(query.record().indexOf("id")));
            activity.insert("user_name", query.value(query.record().indexOf("user_name")));
            activity.insert("content", query.value(query.record().indexOf("content")));
            activity.insert("type", query.value(query.record().indexOf("type")));
            activity.insert("date_created", query.value(query.record().indexOf("date_created")));

            activities.append(activity);
        }

        *ok = true;
        return activities;
    }
    else
    {
        QSqlQuery query;
        query.prepare("SELECT * FROM activities WHERE type = :type AND user_id=:user_id ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (intPage-1)* PER_PAGE);
        query.bindValue(":user_id",curUserId);
        query.bindValue(":type",type);
        if(!query.exec())
            *ok = false;

        QVariantList activities;

        while (query.next()) {
            QVariantMap activity;
            activity.insert("id", query.value(query.record().indexOf("id")));
            activity.insert("user_name", query.value(query.record().indexOf("user_name")));
            activity.insert("content", query.value(query.record().indexOf("content")));
            activity.insert("type", query.value(query.record().indexOf("type")));
            activity.insert("date_created", query.value(query.record().indexOf("date_created")));

            activities.append(activity);
        }

        *ok = true;
        return activities;

    }

}

bool ActivityManager::deleteActivity(uint objectId)
{
    QObject parentObject;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("activities", &parentObject);
    query->setUpdateDates(true);
    query->setType(IDatabaseUpdateQuery::Delete);
    query->singleWhere("object_id",objectId);

    if(!query->executeQuery()){
        return false;
    }

    return true;
}

bool ActivityManager::editActivity(uint objectId, QString content)
{
    QObject parent;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("activities", &parent);
    query->setUpdateDates(true);
    query->singleWhere("object_id", objectId);

    query->setColumnValue("content", content);

    if(query->executeQuery()){
        return true;
    }

    return false;

}



