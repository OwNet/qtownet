#include "groupsservice.h"

#include "irequest.h"
#include "ibus.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "irouter.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QVariant>

GroupsService::GroupsService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void GroupsService::init(IRouter *router)
{
    router->addRoute("/approveUser")->on(IRequest::POST, ROUTE(approveUser) );
    router->addRoute("/joinGroup")->on(IRequest::POST, ROUTE(joinGroup) );
    router->addRoute("/addAdmin")->on(IRequest::POST, ROUTE(addAdmin) );
    router->addRoute("/getUsersGroups")->on(IRequest::POST, ROUTE(getUsersGroups) );
    router->addRoute("/getApprovements")->on(IRequest::POST, ROUTE(getApprovements) );
    router->addRoute("/getGroupUsers")->on(IRequest::POST, ROUTE(getGroupUsers) );
    router->addRoute("/deleteUser")->on(IRequest::POST, ROUTE(deleteUser) );
    router->addRoute("/getGroupTypes")->on(IRequest::POST, ROUTE(getGroupTypes) );

    router->addRoute("/isMember")->on(IRequest::POST, ROUTE_FN {
        return new QVariant(this->isMember(req->parameterValue("user_id").toInt(),req->parameterValue("group_id").toInt()));
    });


    router->addRoute("/isAdmin")->on(IRequest::POST, ROUTE_FN {
        return new QVariant(this->isAdmin(req->parameterValue("user_id").toInt(),req->parameterValue("group_id").toInt()));
    });
}

bool GroupsService::isMember(int user_id, int group_id)
{

    QSqlQuery q_check;

    q_check.prepare("SELECT status FROM group_users WHERE user_id = :user_id AND group_id = :group_id");
    q_check.bindValue(":user_id",user_id);
    q_check.bindValue(":group_id",group_id);
    q_check.exec();

    if(q_check.first() && q_check.value(q_check.record().indexOf("status")) == "1")
        return true;
    else
        return false;

}

bool GroupsService::isAdmin(int user_id, int group_id)
{

    QSqlQuery q_check;

    q_check.prepare("SELECT id FROM group_admins WHERE user_id = :user_id AND group_id = :group_id");
    q_check.bindValue(":user_id",user_id);
    q_check.bindValue(":group_id",group_id);
    q_check.exec();

    return q_check.first();
}

// create element
QVariant* GroupsService::create(IBus *bus, IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;
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

        IDatabaseUpdate *createGroup = m_proxyConnection->databaseUpdate();

        IDatabaseUpdateQuery *query = createGroup->createUpdateQuery("groups", IDatabaseUpdateQuery::Insert);

        query->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        query->setColumnValue("id", id);
        query->setColumnValue("name", name);
        query->setColumnValue("description", description);
        query->setColumnValue("group_type_id", group_type);
        query->setColumnValue("password", password);
        query->setColumnValue("has_password", has_password);
        query->setColumnValue("has_approvement", has_approvement);
        query->setColumnValue("parent",parent);

        int a = createGroup->execute();
        if(a){
            bus->setHttpStatus(500,"Internal server error");
            return new QVariant;
        }

        IDatabaseUpdate *group_admin = m_proxyConnection->databaseUpdate();

        IDatabaseUpdateQuery *q2 = group_admin->createUpdateQuery("group_admins", IDatabaseUpdateQuery::Insert);

        q2->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        q2->setColumnValue("user_id", user_id);
        q2->setColumnValue("group_id", id);

        if(group_admin->execute()){
            bus->setHttpStatus(500,"Internal server error");

            //delete group
            IDatabaseUpdate *del = m_proxyConnection->databaseUpdate();

            IDatabaseUpdateQuery *del_q = del->createUpdateQuery("groups", IDatabaseUpdateQuery::Delete);
            del_q->setUpdateDates(true);
            del_q->setWhere("id", id);
            del->execute();


            return new QVariant;
        }

        IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();


        IDatabaseUpdateQuery *q3 = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Insert);

        q3->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        q3->setColumnValue("user_id", user_id);
        q3->setColumnValue("status","1");
        q3->setColumnValue("group_id", id);

        if(group_user->execute()){
            bus->setHttpStatus(500,"Internal error");

            //delete group and admin

            //delete group
            IDatabaseUpdate *del = m_proxyConnection->databaseUpdate();

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
QVariant* GroupsService::show(IBus *bus, IRequest *req, int id)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM groups WHERE id = :id");
    query.bindValue(":id",id);

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
            q_inner_groups.bindValue(":id",id);

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

QVariant* GroupsService::index( IBus *bus,  IRequest *req)
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


QVariant* GroupsService::edit(IBus *bus, IRequest *req, int id)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(this->isAdmin(curUser_id.toInt(), reqJson["group_id"].toInt())){

            IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();
            IDatabaseUpdateQuery *query = update->createUpdateQuery("groups", IDatabaseUpdateQuery::Update);
            query->setUpdateDates(true);
            query->setWhere("id", reqJson["group_id"]);

            if(reqJson["name"] != "")
                query->setColumnValue("name", reqJson["name"]);

            if(reqJson["description"] != "")
                query->setColumnValue("description", reqJson["description"]);

            if(reqJson["has_approvement"] != "")
                query->setColumnValue("has_approvement", reqJson["has_approvement"]);

            if(reqJson["password"] != "")
                query->setColumnValue("password", reqJson["password"]);


            if(reqJson["has_password"] != ""){
                query->setColumnValue("has_password", reqJson["has_password"]);
                if(reqJson["has_password"] == "1"){
                    if(reqJson["password"] != ""){
                        query->setColumnValue("password", reqJson["password"]);
                    }
                    else{
                        bus->setHttpStatus(400, "Bad Request");
                        return new QVariant;
                    }
                }

            }

            if(reqJson["group_type_id"] != "")
                query->setColumnValue("group_type_id", reqJson["group_type_id"]);

            if(!update->execute()){
                bus->setHttpStatus(200,"OK");
                return new QVariant;
            }
            else{
                bus->setHttpStatus(500, "Internal Server Error");
                return new QVariant;
            }
        }
        else{
            bus->setHttpStatus(400, "Bad Request");
            return new QVariant;
         }

}


QVariant* GroupsService::del(IBus *bus, IRequest *req, int id)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(this->isAdmin(curUser_id.toInt(), reqJson["group_id"].toInt())){

            IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();
            IDatabaseUpdateQuery *query = update->createUpdateQuery("groups", IDatabaseUpdateQuery::Delete);
            query->setUpdateDates(true);
            query->setWhere("id", reqJson["group_id"]);
            if(!update->execute()){
                bus->setHttpStatus(200,"OK");
                return new QVariant;
            }
            else{
                bus->setHttpStatus(500, "Internal Server Error");
                return new QVariant;
            }
     }
     else{
            bus->setHttpStatus(400, "Bad Request");
            return new QVariant;
     }

}

QVariant* GroupsService::joinGroup(IBus *bus, IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QVariantMap error;
    QSqlQuery query;
  //  bool missingValue = false;

    QString group_id = reqJson["group_id"].toString();
    QString user_id = reqJson["user_id"].toString();

    if(user_id == ""){

        error.insert("user_id","required");
        bus->setHttpStatus(400,"Bad Request");
        return new QVariant(error);

    }

    //user exists?
    query.prepare("SELECT * FROM users WHERE id = :id");
    query.bindValue(":id", user_id);

    query.exec();
    if(!query.first()){
         error.insert("error","user_id");
         bus->setHttpStatus(400,"Bad Request");
         return new QVariant(error);
    }


    if(group_id == ""){

        error.insert("group_id","required");
        bus->setHttpStatus(400,"Bad Request");
        return new QVariant(error);

    }

    query.prepare("SELECT * FROM group_users WHERE group_id = :group_id AND user_id = :user_id ");
    query.bindValue(":user_id", user_id);
    query.bindValue(":group_id", group_id);

    query.exec();
    if(query.first()){
       bus->setHttpStatus(301,"Conflict");
       return new QVariant;
    }

    query.prepare("SELECT * FROM groups WHERE id = :id");
    query.bindValue(":id", group_id);


    if( query.exec()){
         if(!query.first()){
             error.insert("error","group_id");
             bus->setHttpStatus(400,"Bad Request");
             return new QVariant(error);

         }

         // if group has password
         if( query.value(query.record().indexOf("has_password")) == "1"){
             QString password = reqJson["password"].toString();

             if(password == ""){

                 error.insert("password","required");
                 bus->setHttpStatus(400,"Bad Request");
                 return new QVariant(error);
             }

             if(password ==  query.value(query.record().indexOf("password") )){
                   //vytvorime zaznam so statusom 1
                 IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();

                 IDatabaseUpdateQuery *q = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Insert);

                 q->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

                 q->setColumnValue("user_id", user_id);
                 q->setColumnValue("group_id", group_id);
                 q->setColumnValue("status", "1");

                 if(group_user->execute()){
                     bus->setHttpStatus(500,"Internal server error");
                 }
             }
             else{
                 error.insert("error","password");
                 bus->setHttpStatus(400,"Bad Request");
                 return new QVariant(error);
             }

         }
         // group has approvement
         else if(query.value(query.record().indexOf("has_approvement")) == "1"){

             IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();
             IDatabaseUpdateQuery *q = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Insert);

             q->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

             q->setColumnValue("user_id", user_id);
             q->setColumnValue("group_id", group_id);
             q->setColumnValue("status", "0");

             if(group_user->execute()){
                 bus->setHttpStatus(500,"Internal server error");
             }
         }
         // group has no password and no approvement
         else{
             IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();
             IDatabaseUpdateQuery *q = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Insert);

             q->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

             q->setColumnValue("user_id", user_id);
             q->setColumnValue("group_id", group_id);
             q->setColumnValue("status", "1");

             if(group_user->execute()){
                 bus->setHttpStatus(500,"Internal server error");
             }
         }


    }
    bus->setHttpStatus(500,"Internal Server Error");
    return new QVariant;
}


QVariant* GroupsService::approveUser(IBus *bus, IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QSqlQuery query;
   //  bool missingValue = false;

    QString group_id = reqJson["group_id"].toString();
    QString user_id = reqJson["user_id"].toString();

    //user exists?
    query.prepare("SELECT * FROM group_users WHERE group_id = :group_id AND user_id = :user_id ");
    query.bindValue(":user_id", user_id);
    query.bindValue(":group_id", group_id);

    if(query.exec()){
        if(!query.first()){
            bus->setHttpStatus(400,"Bad Request");
            return new QVariant;
        }
        query.prepare("SELECT * FROM group_admins WHERE group_id = :group_id AND user_id = :user_id ");
        query.bindValue(":user_id", m_proxyConnection->session()->value("logged"));
        query.bindValue(":group_id", group_id);
        query.exec();

        if( m_proxyConnection->session()->isLoggedIn() && query.first())
        {
            IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();
            IDatabaseUpdateQuery *q = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Update);

            q->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated
            q->setColumnValue("status", "1");

            if(group_user->execute()){
                bus->setHttpStatus(500,"Internal server error");
            }
        }
        else{
            bus->setHttpStatus(400,"Bad Request");
            return new QVariant;
        }
    }

    bus->setHttpStatus(500,"Internal Server Error");
    return new QVariant();

}

QVariant* GroupsService::addAdmin(IBus *bus, IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QSqlQuery query;
   //  bool missingValue = false;

    QString group_id = reqJson["group_id"].toString();
    QString user_id = reqJson["user_id"].toString();

    //user exists?
    query.prepare("SELECT * FROM group_users WHERE group_id = :group_id AND user_id = :user_id ");
    query.bindValue(":user_id", user_id);
    query.bindValue(":group_id", group_id);

    if(query.exec()){
        if(!query.first()){
            bus->setHttpStatus(400,"Bad Request");
            return new QVariant;
        }
        query.prepare("SELECT * FROM group_admins WHERE group_id = :group_id AND user_id = :user_id ");
        query.bindValue(":user_id", m_proxyConnection->session()->value("logged"));
        query.bindValue(":group_id", group_id);
        query.exec();

        if( m_proxyConnection->session()->isLoggedIn() && query.first())
        {
            IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();
            IDatabaseUpdateQuery *q = group_user->createUpdateQuery("group_admins", IDatabaseUpdateQuery::Insert);

            q->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated
            q->setColumnValue("user_id", user_id);
            q->setColumnValue("group_id", group_id);

            if(group_user->execute()){
                bus->setHttpStatus(500,"Internal server error");
            }
        }
        else{
            bus->setHttpStatus(400,"Bad Request");
            return new QVariant;
        }
    }

    bus->setHttpStatus(500,"Internal Server Error");
    return new QVariant();

}

QVariant* GroupsService::getApprovements(IBus *bus, IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;
    QString group_id = reqJson["group_id"].toString();

    QVariantList approvements;
    QSqlQuery query;


    if(this->isAdmin(m_proxyConnection->session()->value("logged").toInt(), group_id.toInt())){
        query.prepare("SELECT user_id FROM group_users WHERE group_id = :group_id AND status = 0");
        query.bindValue(":group_id", group_id);
        query.exec();

        while(query.next()){
           QVariantMap approvement;
           approvement.insert("user_id",query.value(query.record().indexOf("user_id")));
           approvement.insert("group_id",group_id);

           approvements.append(approvement);
        }
    }

    bus->setHttpStatus(200,"OK");
    return new QVariant(approvements);

}

QVariant* GroupsService::getGroupUsers(IBus *bus, IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;
    QString group_id = reqJson["group_id"].toString();

    QVariantList users;
    QSqlQuery query;


    if(this->isMember(m_proxyConnection->session()->value("logged").toInt(), group_id.toInt())){


        query.prepare("SELECT * FROM users"
                      "INNER JOIN group_users ON users.id = group_users.user_id"
                      "WHERE group_users.status = 1 AND group_users.group_id = :group_id");
        query.bindValue(":group_id", group_id);
        query.exec();

        while(query.next()){

           QVariantMap user;
           user.insert("first_name",query.value(query.record().indexOf("first_name")));
           user.insert("last_name",query.value(query.record().indexOf("last_name")));
           if(this->isAdmin(query.value(query.record().indexOf("id")).toInt(),group_id.toInt()))
               user.insert("isAdmin","1");
           else
               user.insert("isAdmin","0");

           users.append(user);
        }
        bus->setHttpStatus(200,"OK");
        return new QVariant(users);
    }
    bus->setHttpStatus(400,"Bad Request");
    return new QVariant();

}

QVariant* GroupsService::getUsersGroups(IBus *bus, IRequest *req)
{
    QVariantList groups;
    QSqlQuery query;

    if(m_proxyConnection->session()->value("logged") !=""){


        query.prepare("SELECT * FROM groups"
                      "INNER JOIN group_users ON groups.id = group_users.group_id"
                      "WHERE group_users.status = 1 AND group_users.user_id = :user_id");
        query.bindValue(":user_id", m_proxyConnection->session()->value("logged").toInt());
        query.exec();

        while(query.next()){

           QVariantMap group;
           group.insert("first_name",query.value(query.record().indexOf("user_id")));
           group.insert("lastn_name",query.value(query.record().indexOf("last_name")));

           groups.append(group);
        }

        bus->setHttpStatus(200,"OK");
        return new QVariant(groups);
    }
    bus->setHttpStatus(400,"Bad Request");
    return new QVariant();


}

QVariant* GroupsService::getGroupTypes(IBus *bus, IRequest *req)
{
    QVariantList types;
    QSqlQuery query;

    if(m_proxyConnection->session()->value("logged") !=""){


        query.prepare("SELECT * FROM group_types");
        query.exec();

        while(query.next()){

           QVariantMap type;
           type.insert("id",query.value(query.record().indexOf("id")));
           type.insert("name",query.value(query.record().indexOf("name")));

           types.append(type);
        }

        bus->setHttpStatus(200,"OK");
        return new QVariant(types);
    }
    bus->setHttpStatus(400,"Bad Request");
    return new QVariant();


}

QVariant* GroupsService::deleteUser(IBus *bus, IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QSqlQuery query1;
    bool allowDelete = false;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(curUser_id == reqJson["user_id"] )
        allowDelete = true;
    else{
        if(this->isAdmin(curUser_id.toInt(), reqJson["group_id"].toInt()))
          allowDelete = true;
    }
    if(allowDelete){

        query1.prepare("SELECT * FROM group_users WHERE user_id = :user_id AND group_id = :group_id");
        query1.bindValue(":user_id", reqJson["user_id"]);
        query1.bindValue(":group_id", reqJson["group_id"]);
        query1.exec();
        if(query1.first()){

            IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();

            IDatabaseUpdateQuery *query = update->createUpdateQuery("group_users", IDatabaseUpdateQuery::Delete);
            query->setUpdateDates(true);
            query->setWhere("user_id", reqJson["user_id"]);
            query->setWhere("group_id",reqJson["group_id"]);
            update->execute();

            bus->setHttpStatus(200,"OK");
            return new QVariant;

        }
        else{
            bus->setHttpStatus(400, "Bad Request");
            return new QVariant;
        }
    }
    else{
        bus->setHttpStatus(400, "Bad Request");
        return new QVariant;
    }

}
