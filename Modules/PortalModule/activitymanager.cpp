#include "activitymanager.h"
#include <QSqlQuery>
#include <QSqlRecord>

#include "idatabaseupdatequery.h"
#include "iproxyconnection.h"
#include "isession.h"

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
    query->setColumnValue("object_id", ac.object_id);

    if(!query->executeQuery()){
        return false;
    }
    else
    {
        return true;
    }
}

QVariantList ActivityManager::getActivities(bool *ok)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM activities");

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

bool ActivityManager::deleteActivity(QString objectId)
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

bool ActivityManager::editActivity(QString objectId, QString content)
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


