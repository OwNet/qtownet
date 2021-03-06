#include "groupmodule.h"

#include <QSqlQuery>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QVariant>
#include <QDateTime>
#include <QHash>
#include <QSqlRecord>

#include "databaseupdate.h"
#include "qjson/parser.h"
#include "qjson/serializer.h"
#include "irequest.h"

GroupModule::GroupModule(QObject *parent) :
    QObject(parent)
{
}


// create element
QVariant* GroupModule::create(IBus *bus, IRequest *req)
{
    QVariantMap reqJson = req->postBodyFromJson();
    QVariantMap error;

    bool missingValue = false;

    QString name = reqJson["name"].toString();
    if(name == ""){
        missingValue = true;
        error.insert("name","required");

    }
    else{

        // check if group with stated name exists
        QSqlQuery q_check;

        q_check.prepare("SELECT name FROM groups WHERE name = :name");
        q_check.bindValue(":name",name);
        q_check.exec();

        if(q_check.first()){
            bus->setHttpStatus(409, "Conflict");
            // create answer
            QVariantMap status;
            status.insert("error", name);

            return new QVariant(status);
        }
    }

    QString user_id = reqJson["user_id"].toString();
    if(user_id == ""){
        missingValue = true;
        error.insert("user_id","required");
    }

    QString group_type = reqJson["group_type"].toString();
    if(group_type == ""){
        missingValue = true;
        error.insert("group_type","required");
    }

    QString description = reqJson["description"].toString();
    if(description == ""){
        missingValue = true;
        error.insert("description","required");
    }

    QString has_password = reqJson["has_password"].toString();
    if(has_password == ""){
        missingValue = true;
        error.insert("has_password","required");
    }

    QString password = reqJson["password"].toString();
    if(password == "" && has_password!="0" ){
        missingValue = true;
        error.insert("password","required");
    }

    QString parent = reqJson["parent"].toString();
    if(parent != ""){

        // check if group with stated id exists and if it already have parent
        QSqlQuery q_check;

        q_check.prepare("SELECT * FROM groups WHERE id = :parent");
        q_check.bindValue(":parent",parent);
        q_check.exec();

        if(!q_check.first()){
            bus->setHttpStatus(400, "Bad request");
            // create answer
            QVariantMap status;
            status.insert("error", "parent group does not exist");

            return new QVariant(status);
        }
        if(q_check.value(q_check.record().indexOf("parent")) !=""){
            bus->setHttpStatus(400, "Bad request");
            // create answer
            QVariantMap status;
            status.insert("error", "not allowed nesting level");

            return new QVariant(status);
        }

    }


    QString has_approvement = reqJson["has_approvement"].toString();
    if(has_approvement == ""){
        missingValue = true;
        error.insert("has_approvement","required");
    }

    // missing argument

    if(missingValue){

        bus->setHttpStatus(400,"Bad Request");

        return new QVariant(error);
    }
    else{

        //creating group ID
        uint id = qHash(QString("%1-%2").arg(name).arg(QDateTime::currentDateTime().toString(Qt::ISODate)));

        DatabaseUpdate createGroup;

        IDatabaseUpdateQuery *query = createGroup.createUpdateQuery("groups", IDatabaseUpdateQuery::Insert);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        query->setColumnValue("id", id);
        query->setColumnValue("name", name);
        query->setColumnValue("description", description);
        query->setColumnValue("group_type_id", group_type);
        query->setColumnValue("password", password);
        query->setColumnValue("has_password", has_password);
        query->setColumnValue("has_approvement", has_approvement);
        query->setColumnValue("parent",parent);

        int a = createGroup.execute();
        if(a){
            bus->setHttpStatus(500,"Internal server error");
            return new QVariant;
        }

        IDatabaseUpdate *group_admin = bus->databaseUpdate();

        IDatabaseUpdateQuery *q2 = group_admin->createUpdateQuery("group_admins", IDatabaseUpdateQuery::Insert);

        q2->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        q2->setColumnValue("user_id", user_id);
        q2->setColumnValue("group_id", id);

        if(group_admin->execute()){
            bus->setHttpStatus(500,"Internal server error");

            //delete group
            IDatabaseUpdate *del = bus->databaseUpdate();

            IDatabaseUpdateQuery *del_q = del->createUpdateQuery("groups", IDatabaseUpdateQuery::Delete);
            del_q->setUpdateDates(true);
            del_q->setWhere("id", id);
            del->execute();


            return new QVariant;
        }

        IDatabaseUpdate *group_user = bus->databaseUpdate();


        IDatabaseUpdateQuery *q3 = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Insert);

        q3->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        q3->setColumnValue("user_id", user_id);
        q3->setColumnValue("status","1");
        q3->setColumnValue("group_id", id);

        if(group_user->execute()){
            bus->setHttpStatus(500,"Internal error");

            //delete group and admin

            //delete group
            IDatabaseUpdate *del = bus->databaseUpdate();

            IDatabaseUpdateQuery *del_q = del->createUpdateQuery("groups", IDatabaseUpdateQuery::Delete);
            del_q->setUpdateDates(true);
            del_q->setWhere("id", id);

            //delete admin_group

            IDatabaseUpdateQuery *del_q2 = del->createUpdateQuery("group_admins", IDatabaseUpdateQuery::Delete);
            del_q2->setUpdateDates(true);
            del_q2->setWhere("group_id", id);
            del->execute();

            return new QVariant;
        }
    }

    bus->setHttpStatus(201, "Created");
    return new QVariant;
}

// show element
QVariant* GroupModule::show( IBus *bus, IRequest *req)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM groups WHERE id = :id");
    query.bindValue(":id",req->id());

    if( query.exec()) {
        QVariantList groupDetail;
        QVariantMap group;

        if(query.first()){

            bus->setHttpStatus(200, "OK");

            group.insert("id", query.value(query.record().indexOf("id")));
            group.insert("name", query.value(query.record().indexOf("name")));
            group.insert("description", query.value(query.record().indexOf("description")));
            group.insert("has_password", query.value(query.record().indexOf("has_password")));
            group.insert("has_approvement", query.value(query.record().indexOf("has_approvement")));

            QSqlQuery q_type;

            q_type.prepare("SELECT name FROM group_types WHERE id = :id");
            q_type.bindValue(":id",query.value(query.record().indexOf("group_type_id")));

            if(!q_type.exec()){
                bus->setHttpStatus(500,"Internal Server error");
                return new QVariant;
            }

            q_type.first();
            group.insert("group_type_name", q_type.value(q_type.record().indexOf("name")));

            QSqlQuery q_inner_groups;

            q_inner_groups.prepare("SELECT * FROM groups WHERE parent = :id");
            q_inner_groups.bindValue(":id",req->id());

            if(!q_inner_groups.exec()){
                bus->setHttpStatus(500,"Internal Server error");
                return new QVariant;
            }

            groupDetail.append(group);

            while(q_inner_groups.next()){
                QVariantMap innerGroup;

                innerGroup.insert("name",q_inner_groups.value(q_inner_groups.record().indexOf("name")));
                innerGroup.insert("id",q_inner_groups.value(q_inner_groups.record().indexOf("id")));

                groupDetail.append(innerGroup);
            }

            return new QVariant(groupDetail);
        }
        bus->setHttpStatus(400,"Bad Request");
        return new QVariant;
    }

    bus->setHttpStatus(500,"Internal Server error");
    return new QVariant;
}

QVariant* GroupModule::index( IBus *bus,  IRequest *req)
{

    QSqlQuery query;

    if( query.exec("SELECT * FROM groups")){
        QVariantList groups;

        while(query.next())
        {
            QVariantMap group;

            group.insert("id", query.value(query.record().indexOf("id")));
            group.insert("name", query.value(query.record().indexOf("name")));
            group.insert("description", query.value(query.record().indexOf("description")));
            group.insert("has_password", query.value(query.record().indexOf("has_password")));
            group.insert("has_approvement", query.value(query.record().indexOf("has_approvement")));

            groups.append(group);
        }

        bus->setHttpStatus(200, "OK");
        return new QVariant(groups);
    }
    else
        bus->setHttpStatus(500,"Internal Server error");

    return new QVariant;
}
