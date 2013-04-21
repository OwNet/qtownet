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

    QSqlQuery queryName;
    queryName.prepare("SELECT * FROM users WHERE id = :id");
    queryName.bindValue(":id",ac.user_id);

    if(!queryName.exec())
        return false;

    queryName.first();
    ac.user_name =  queryName.value(queryName.record().indexOf("first_name")).toString() + QString(" ") +  queryName.value(queryName.record().indexOf("last_name")).toString();
    ac.gender = queryName.value(queryName.record().indexOf("gender")).toString();

    QObject parentObject;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("activities", &parentObject);

    query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

    query->setColumnValue("user_name", ac.user_name);
    query->setColumnValue("content", ac.content);
    query->setColumnValue("type", ac.activity_type);
    query->setColumnValue("group_id", ac.group_id);
    query->setColumnValue("user_id", ac.user_id);
    query->setColumnValue("object_id", ac.object_id);
    query->setColumnValue("gender", ac.gender);

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
    QSqlQuery query;

    if(type == "")
    {
        query.prepare("SELECT COUNT(*) AS n FROM activities");   
    }
    else{
        query.prepare("SELECT COUNT(*) AS n FROM activities WHERE type=:type");
        query.bindValue(":type",type);
    }

    if(query.exec()){
        query.first();
        int x =  query.value(query.record().indexOf("n")).toInt();
        return qCeil(x/(double)PER_PAGE);
    }
    return 0;
}


QVariantList ActivityManager::getActivities(bool *ok, QVariantMap &error, IRequest *req)
{


    QString type = req->parameterValue("type");
    QSqlQuery query;

    int intPage;
    if(!(intPage= req->parameterValue("page").toInt())){
        error.insert("page_number","error");
    }


    if(type == "")
    {

        query.prepare("SELECT * FROM activities ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (intPage-1)* PER_PAGE);
    }
    else
    {
        query.prepare("SELECT * FROM activities WHERE type = :type ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (intPage-1)* PER_PAGE);
        query.bindValue(":type",type);
    }

    if(!query.exec())
        *ok = false;
    QVariantList activities;

    while (query.next()) {
        QVariantMap activity;
        activity.insert("object_id", query.value(query.record().indexOf("object_id")).toString());
        activity.insert("user_id", query.value(query.record().indexOf("user_id")).toString());
        activity.insert("user_name", query.value(query.record().indexOf("user_name")).toString());

        if(query.value(query.record().indexOf("type")).toString() == QString::number(Activity::MESSAGE)){
            QSqlQuery queryMessages;
            queryMessages.prepare("SELECT * FROM messages WHERE parent_id = :uid");
            queryMessages.bindValue(":uid", query.value(query.record().indexOf("object_id")).toString());
            queryMessages.exec();
            int i = 0;
            while(queryMessages.next())
                i++;
            QString content = query.value(query.record().indexOf("content")).toString() + QString(";") + QString::number(i);
            activity.insert("content",content);
        }
        else
            activity.insert("content", query.value(query.record().indexOf("content")).toString());

        activity.insert("type", query.value(query.record().indexOf("type")).toString());
        activity.insert("date_created", query.value(query.record().indexOf("date_created")).toString());
        activity.insert("gender", query.value(query.record().indexOf("gender")).toString());

        activities.append(activity);
    }

    *ok = true;
    return activities;

}


int ActivityManager::usersPagesCount(IRequest *req)
{

    QString userId;
    if(req->parameterValue("user_id") == "")
        userId = m_proxyConnection->session()->value("logged").toString();
    else
        userId = req->parameterValue("user_id");

    //TODO add some validations + error response trought parameter

    QString type = req->parameterValue("type");
    QSqlQuery query;

    if(type == "")
    {
        query.prepare("SELECT COUNT(*) AS n FROM activities WHERE user_id = :user_id");
        query.bindValue(":user_id",userId);

    }
    else{

        query.prepare("SELECT COUNT(*) AS n FROM activities WHERE type=:type AND user_id=:user_id");
        query.bindValue(":type",type);
        query.bindValue(":user_id",userId);

    }
    if(query.exec()){
        query.first();
        int x =  query.value(query.record().indexOf("n")).toInt();
        return qCeil((double)x/(double)PER_PAGE);
    }
    else
        return 0;
}




QVariantList ActivityManager::getUsersActivities(bool *ok, QVariantMap &error, IRequest *req)
{
    QString userId;
    if(req->parameterValue("user_id") == "")
        userId = m_proxyConnection->session()->value("logged").toString();
    else
        userId = req->parameterValue("user_id");

    QString type = req->parameterValue("type");
    QSqlQuery query;

    int intPage;
    if(!(intPage= req->parameterValue("page").toInt())){
        error.insert("page_number","error");
    }


    if(type == "")
    {
        query.prepare("SELECT * FROM activities WHERE user_id = :user_id ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":user_id",userId);
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (intPage-1)* PER_PAGE);

    }
    else
    {

        query.prepare("SELECT * FROM activities WHERE type = :type AND user_id=:user_id ORDER BY date_created DESC LIMIT :limit OFFSET :offset");
        query.bindValue(":limit",PER_PAGE);
        query.bindValue(":offset", (intPage-1)* PER_PAGE);
        query.bindValue(":user_id",userId);
        query.bindValue(":type",type);
    }
    if(!query.exec())
        *ok = false;

    QVariantList activities;

    while (query.next()) {
        QVariantMap activity;
        activity.insert("object_id", query.value(query.record().indexOf("object_id")).toString());
        activity.insert("user_id", query.value(query.record().indexOf("user_id")).toString());
        activity.insert("user_name", query.value(query.record().indexOf("user_name")));

        if(query.value(query.record().indexOf("type")).toString() == QString::number(Activity::MESSAGE)){
            QSqlQuery queryMessages;
            queryMessages.prepare("SELECT * FROM messages WHERE parent_id = :uid");
            queryMessages.bindValue(":uid", query.value(query.record().indexOf("object_id")).toString());
            queryMessages.exec();
            int i = 0;
            while(queryMessages.next())
                i++;
            QString content = query.value(query.record().indexOf("content")).toString() + QString(";") + QString::number(i);
            activity.insert("content",content);
        }
        else
            activity.insert("content", query.value(query.record().indexOf("content")).toString());

        activity.insert("type", query.value(query.record().indexOf("type")).toString());
        activity.insert("date_created", query.value(query.record().indexOf("date_created")).toString());
        activity.insert("gender", query.value(query.record().indexOf("gender")).toString());

        activities.append(activity);
    }

    *ok = true;
    return activities;
}

bool ActivityManager::deleteActivity(const QString &objectId)
{
    QObject parentObject;
    IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("activities", &parentObject);
    //query->setUpdateDates(true);
    query->setType(IDatabaseUpdateQuery::Delete);
    query->singleWhere("object_id",objectId);

    if(!query->executeQuery()){
        return false;
    }

    return true;
}

bool ActivityManager::editActivity(const QString &objectId, QString content)
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



