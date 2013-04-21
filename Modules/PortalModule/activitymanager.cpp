#include "activitymanager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>

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

    QSqlQuery query = buildQuery(req, true);

    if(query.exec()){
        query.first();
        int x =  query.value("count").toInt();
        return qCeil(x/(double)PER_PAGE);
    }

    return 0;
}


QVariantList ActivityManager::getActivities(bool *ok, QVariantMap &error, IRequest *req)
{    
    QSqlQuery query = buildQuery(req, false);
    QVariantList activities;

    if (!query.exec()) {
        ok = false;
        return activities;
    }

    while (query.next()) {
        QVariantMap activity;
        activity.insert("object_id", query.value("object_id").toString());
        activity.insert("user_id", query.value("user_id").toString());
        activity.insert("user_name", query.value("user_name").toString());

        int type = query.value("type").toInt();
        if (type == Activity::MESSAGE) {
            QSqlQuery queryMessages;
            queryMessages.prepare("SELECT count(*) as count FROM messages WHERE parent_id = :uid");
            queryMessages.bindValue(":uid", query.value("object_id").toString());
            queryMessages.exec();
            queryMessages.first();

            QString content = query.value("content").toString() + QString(";") + queryMessages.value("count").toString();
            activity.insert("content",content);
        }
        else
            activity.insert("content", query.value("content").toString());

        activity.insert("type", type);
        activity.insert("date_created", query.value("date_created").toString());
        activity.insert("gender", query.value("gender").toString());

        activities.append(activity);
    }

    *ok = true;
    return activities;
}

QSqlQuery ActivityManager::buildQuery(IRequest *req, bool count)
{
    QString q;
    bool hasType = req->hasParameter("type");
    bool hasUser = req->hasParameter("user_id");
    bool hasGroup = req->hasParameter("group_id");
    bool hasPage = req->hasParameter("page");

    q =  "SELECT ";
    q += count ? "count(*) as count " : "* ";
    q += "FROM activities ";

    if ( hasType || hasUser || hasGroup) {
        q += "WHERE ";
        QStringList list;
        if (hasType) list.append("type=:type");
        if (hasUser) list.append("user_id=:user_id");
        if (hasGroup) list.append("group_id=:group_id");
        q += list.join(" AND ") + " ";
    }

    if (!count)
        q += "ORDER BY date_created DESC ";

    if (hasPage)
        q += "LIMIT :limit OFFSET :offset ";

    QSqlQuery query;
    query.prepare(q);

    if (hasType) query.bindValue(":type", req->parameterValue("type"));
    if (hasUser) query.bindValue(":user_id", req->parameterValue("user_id"));
    if (hasGroup) query.bindValue(":group_id", req->parameterValue("group_id"));
    if (hasPage) {
        query.bindValue(":limit", PER_PAGE);
        query.bindValue(":offset", (req->parameterValue("page").toInt() -1) * PER_PAGE);
    }

    return query;
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



