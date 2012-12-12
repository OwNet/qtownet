#include "groupsservice.h"

#include "irequest.h"
#include "idatabaseupdate.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "irouter.h"
#include "portalhelper.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QVariant>
#include <QCryptographicHash>


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
    router->addRoute("/isAdmin")->on(IRequest::POST, ROUTE(getIsAdmin));
    router->addRoute("/isMember")->on(IRequest::POST, ROUTE(getIsMember));
 }


IResponse *GroupsService::getIsAdmin(IRequest *req){


         QVariantMap reqJson = req->postBodyFromJson().toMap();
         QString user_id = reqJson["user_id"].toString();
         QString group_id = reqJson["group_id"].toString();

         QVariantMap res;

         if(isAdmin(user_id.toUInt(), group_id.toUInt()))
             res.insert("admin","1");
         else
             res.insert("admin","0");

         return req->response(QVariant(res),IResponse::OK);
}

IResponse *GroupsService::getIsMember(IRequest *req){


         QVariantMap reqJson = req->postBodyFromJson().toMap();
         QString user_id = reqJson["user_id"].toString();
         QString group_id = reqJson["group_id"].toString();

         QVariantMap res;

         if(isMember(user_id.toUInt(), group_id.toUInt()))
             res.insert("member","1");
         else
             res.insert("member","0");

         return req->response(QVariant(res),IResponse::OK);
}

bool GroupsService::isMember(uint user_id, uint group_id)
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

bool GroupsService::isAdmin(uint user_id, uint group_id)
{

    QSqlQuery q_check;

    q_check.prepare("SELECT id FROM group_admins WHERE user_id = :user_id AND group_id = :group_id");
    q_check.bindValue(":user_id",user_id);
    q_check.bindValue(":group_id",group_id);
    q_check.exec();

    return q_check.first();
}

bool GroupsService::checkGroupPassword(QString password, QString group_id)
{

    QSqlQuery query;

    query.prepare("SELECT * FROM groups WHERE id = :group_id");
    query.bindValue(":group_id",group_id);
    if(!query.exec()){
        return false;
    }
    QString salt = query.value(query.record().indexOf("salt")).toString();
    QString group_password = query.value(query.record().indexOf("password")).toString();

    QByteArray pass_plus_salt = (password+salt).toLatin1();
    QString salted_pass(QCryptographicHash::hash(pass_plus_salt,QCryptographicHash::Sha1).toHex());

    if(salted_pass == group_password)
        return true;
    else
        return false;

}

// create element
IResponse *GroupsService::create( IRequest *req)
{
    QVariantMap reqJson = req->postBodyFromJson().toMap();

    QVariantMap error;
    QString salt = "";

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
            // create answer
            QVariantMap status;
            status.insert("error", name);

            return req->response(QVariant(status), IResponse::CONFLICT );
        }
    }


    QString user_id = m_proxyConnection->session()->value("logged").toString();
    if(user_id==""){
        return req->response(IResponse::FORBIDDEN);

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
    if(has_password == "0")
        password = "";

    // create salt and has password
    else{
        PortalHelper::addSalt(&password, &salt);
    }

    QString parent = reqJson["parent"].toString();
    if(parent != ""){

        // check if group with stated id exists and if it already have parent
        QSqlQuery q_check;

        q_check.prepare("SELECT * FROM groups WHERE id = :parent");
        q_check.bindValue(":parent",parent);
        q_check.exec();

        if(!q_check.first()){
            error.insert("error", "parent group does not exist");
            missingValue = true;

        }
        if(q_check.value(q_check.record().indexOf("parent")) !=""){
            // create answer
            missingValue = true;
            error.insert("nesting_error", "not allowed nesting level");
        }

    }


    QString has_approvement = reqJson["has_approvement"].toString();
    if(has_approvement == ""){
        missingValue = true;
        error.insert("has_approvement","required");
    }

    // if missing argument or any other error
    if(missingValue){

        return req->response(QVariant(error), IResponse::BAD_REQUEST);
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
        query->setColumnValue("salt", salt);

        if(createGroup->execute()){
             return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }

        IDatabaseUpdate *group_admin = m_proxyConnection->databaseUpdate();

        IDatabaseUpdateQuery *q2 = group_admin->createUpdateQuery("group_admins", IDatabaseUpdateQuery::Insert);

        q2->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        q2->setColumnValue("user_id", user_id);
        q2->setColumnValue("group_id", id);

        if(group_admin->execute()){

            //delete group
            IDatabaseUpdate *del = m_proxyConnection->databaseUpdate();

            IDatabaseUpdateQuery *del_q = del->createUpdateQuery("groups", IDatabaseUpdateQuery::Delete);
            del_q->setUpdateDates(true);
            del_q->setWhere("id", id);
            del->execute();

            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }

        IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();


        IDatabaseUpdateQuery *q3 = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Insert);

        q3->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

        q3->setColumnValue("user_id", user_id);
        q3->setColumnValue("status","1");
        q3->setColumnValue("group_id", id);

        if(group_user->execute()){


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

           return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }
    }
    return req->response(IResponse::CREATED);
}

// show element
IResponse *GroupsService::show(IRequest *req, uint id)
{
    QSqlQuery query;

    if(!m_proxyConnection->session()->isLoggedIn())
        req->response(IResponse::FORBIDDEN);

    query.prepare("SELECT * FROM groups WHERE id = :id");
    query.bindValue(":id",id);

    if( !query.exec())
        return req->response(IResponse::INTERNAL_SERVER_ERROR);

    QVariantList groupDetail;
    QVariantMap group;

    if(query.first()){

        group.insert("id", query.value(query.record().indexOf("id")));
        group.insert("name", query.value(query.record().indexOf("name")));
        group.insert("description", query.value(query.record().indexOf("description")));
        group.insert("has_password", query.value(query.record().indexOf("has_password")));
        group.insert("has_approvement", query.value(query.record().indexOf("has_approvement")));

        QSqlQuery q_type;

        q_type.prepare("SELECT name FROM group_types WHERE id = :id");
        q_type.bindValue(":id",query.value(query.record().indexOf("group_type_id")));

        if(!q_type.exec()){
            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }

        q_type.first();
        group.insert("group_type_name", q_type.value(q_type.record().indexOf("name")));

        QSqlQuery q_inner_groups;

        q_inner_groups.prepare("SELECT * FROM groups WHERE parent = :id");
        q_inner_groups.bindValue(":id",id);

        if(!q_inner_groups.exec()){
           return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }

        groupDetail.append(group);

        while(q_inner_groups.next()){
            QVariantMap innerGroup;

            innerGroup.insert("name",q_inner_groups.value(q_inner_groups.record().indexOf("name")));
            innerGroup.insert("id",q_inner_groups.value(q_inner_groups.record().indexOf("id")));

            groupDetail.append(innerGroup);
        }

        return req->response(QVariant(groupDetail), IResponse::OK);
    }
    return req->response(IResponse::BAD_REQUEST);

}

IResponse *GroupsService::index(IRequest *req)
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
        return req->response(QVariant(groups), IResponse::OK);
    }
    else
        return req->response(IResponse::INTERNAL_SERVER_ERROR);
}


IResponse *GroupsService::edit(IRequest *req, uint id)
{

    QVariantMap reqJson = req->postBodyFromJson().toMap();
    QString curUser_id = m_proxyConnection->session()->value("logged").toString();
    QString salt = "";
    QString password = "";

    if(this->isAdmin(curUser_id.toUInt(), id)){

            IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();
            IDatabaseUpdateQuery *query = update->createUpdateQuery("groups", IDatabaseUpdateQuery::Update);
            query->setUpdateDates(true);
            query->setWhere("id", id);

            if(reqJson["name"] != "")
                query->setColumnValue("name", reqJson["name"]);

            if(reqJson["description"] != "")
                query->setColumnValue("description", reqJson["description"]);

            if(reqJson["has_approvement"] != "")
                query->setColumnValue("has_approvement", reqJson["has_approvement"]);

            if(reqJson["password"] != ""){
                password = reqJson["password"].toString();
                PortalHelper::addSalt(&password,&salt);
                query->setColumnValue("password", password);
                query->setColumnValue("salt", salt);

            }
            if(reqJson["has_password"] != ""){
                query->setColumnValue("has_password", reqJson["has_password"]);
                if(reqJson["has_password"] == "1"){
                    if(reqJson["password"] != ""){
                        query->setColumnValue("password", reqJson["password"]);
                    }
                    else{
                        QVariantMap error;
                        error.insert("password","required");
                        return req->response(error, IResponse::BAD_REQUEST);
                    }
                }

            }

            if(reqJson["group_type_id"] != "")
                query->setColumnValue("group_type_id", reqJson["group_type_id"]);

            if(!update->execute()){
                return req->response(IResponse::OK);
            }
            else{
                return req->response(IResponse::INTERNAL_SERVER_ERROR);
            }
        }
        else{
            return req->response(IResponse::FORBIDDEN);
         }

}


IResponse *GroupsService::del(IRequest *req, uint id)
{

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(this->isAdmin(curUser_id.toUInt(), id)){

            IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();
            IDatabaseUpdateQuery *query = update->createUpdateQuery("groups", IDatabaseUpdateQuery::Delete);
            query->setUpdateDates(true);
            query->setWhere("id", id);
            if(!update->execute()){
                return req->response(IResponse::OK);
            }
            else{
                return req->response(IResponse::INTERNAL_SERVER_ERROR);
            }
     }
     else{
            return req->response(IResponse::FORBIDDEN);
     }

}

IResponse *GroupsService::joinGroup(IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QVariantMap error;
    QSqlQuery query;

    QString group_id = reqJson["group_id"].toString();
    QString user_id = m_proxyConnection->session()->value("logged").toString();

    if(group_id == ""){

        error.insert("group_id","required");
        return req->response(QVariant(error),IResponse::BAD_REQUEST);

    }

    query.prepare("SELECT * FROM group_users WHERE user_id=:user_id");
    query.bindValue(":user_id",user_id);
    if(!query.exec())
        req->response(IResponse::INTERNAL_SERVER_ERROR);

    if(this->isMember(user_id.toUInt(),group_id.toUInt()) || query.value(query.record().indexOf("status")).toString() == "0" ){
       req->response(IResponse::CONFLICT);
    }

    query.prepare("SELECT * FROM groups WHERE id = :id");
    query.bindValue(":id", group_id);


    if(!query.exec()){
        if(!query.first()){
             error.insert("error","group_id");
             return req->response(QVariant(error),IResponse::BAD_REQUEST);

        }
        return req->response(QVariant(error),IResponse::INTERNAL_SERVER_ERROR);
    }


         // if group has password
         if( query.value(query.record().indexOf("has_password")) == "1"){
             QString password = reqJson["password"].toString();

             if(password == ""){

                 error.insert("password","required");
                 return req->response(QVariant(error),IResponse::BAD_REQUEST);
             }

             if(this->checkGroupPassword(password,group_id)){
                   //vytvorime zaznam so statusom 1
                 IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();

                 IDatabaseUpdateQuery *q = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Insert);

                 q->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated

                 q->setColumnValue("user_id", user_id);
                 q->setColumnValue("group_id", group_id);
                 q->setColumnValue("status", "1");

                 if(group_user->execute()){
                     return req->response(IResponse::INTERNAL_SERVER_ERROR);
                 }
             }
             else{
                 error.insert("error","password");
                 return req->response(QVariant(error),IResponse::BAD_REQUEST);
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
                 return req->response(QVariant(error),IResponse::INTERNAL_SERVER_ERROR);
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
                 return req->response(QVariant(error),IResponse::INTERNAL_SERVER_ERROR);
             }
         }
        return req->response(QVariant(error),IResponse::OK);


}


IResponse * GroupsService::approveUser( IRequest *req)
{
    bool ok = false;
    QVariantMap reqJson = req->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    QSqlQuery query;
   //  bool missingValue = false;

    QString group_id = reqJson["group_id"].toString();
    QString user_id = reqJson["user_id"].toString();



    if(!this->isMember(user_id.toUInt(),group_id.toUInt())){
        QVariantMap error;
        error.insert("error", "group or user does not exist, or user is not awaiting for approvement");
        return req->response(QVariant(error),IResponse::BAD_REQUEST);

    }
        query.prepare("SELECT * FROM group_admins WHERE group_id = :group_id AND user_id = :user_id ");
        query.bindValue(":user_id", m_proxyConnection->session()->value("logged"));
        query.bindValue(":group_id", group_id);
        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        if( m_proxyConnection->session()->isLoggedIn() && query.first())
        {
            IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();
            IDatabaseUpdateQuery *q = group_user->createUpdateQuery("group_users", IDatabaseUpdateQuery::Update);

            q->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated
            q->setColumnValue("status", "1");

            if(group_user->execute()){
               return req->response(IResponse::INTERNAL_SERVER_ERROR);
            }
        }
        else{

            req->response(IResponse::FORBIDDEN);
        }

        return req->response(IResponse::OK);

}

IResponse * GroupsService::addAdmin(IRequest *req)
{

    QVariantMap reqJson = req->postBodyFromJson().toMap();

    QString group_id = reqJson["group_id"].toString();
    QString user_id = reqJson["user_id"].toString();


    if(!this->isMember(user_id.toUInt(),group_id.toUInt())){
            QVariantMap error;
            error.insert("error","User must be a member of the group");
            return req->response(error,IResponse::BAD_REQUEST);
    }

    if( this->isAdmin(m_proxyConnection->session()->value("logged").toUInt(),group_id.toUInt()))
    {
        IDatabaseUpdate *group_user = m_proxyConnection->databaseUpdate();
        IDatabaseUpdateQuery *q = group_user->createUpdateQuery("group_admins", IDatabaseUpdateQuery::Insert);

        q->setUpdateDates(true); // sam nastavi v tabulke datumy date_created a date_updated
        q->setColumnValue("user_id", user_id);
        q->setColumnValue("group_id", group_id);

        if(group_user->execute()){
            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        }
    }
    else{
        return req->response(IResponse::FORBIDDEN);
    }

    return req->response(IResponse::OK);

}

IResponse *GroupsService::getApprovements(IRequest *req)
{

    QVariantMap reqJson = req->postBodyFromJson().toMap();
    QString group_id = reqJson["group_id"].toString();

    QVariantList approvements;
    QSqlQuery query;


    if(this->isAdmin(m_proxyConnection->session()->value("logged").toUInt(), group_id.toUInt())){
        query.prepare("SELECT user_id FROM group_users WHERE group_id = :group_id AND status = 0");
        query.bindValue(":group_id", group_id);
        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        while(query.next()){
           QVariantMap approvement;
           approvement.insert("user_id",query.value(query.record().indexOf("user_id")));
           approvement.insert("group_id",group_id);

           approvements.append(approvement);
        }
    }
    return req->response(QVariant(approvements),IResponse::OK);
}

IResponse *GroupsService::getGroupUsers( IRequest *req)
{

    QVariantMap reqJson = req->postBodyFromJson().toMap();

    QString group_id = reqJson["group_id"].toString();

    QVariantList users;
    QSqlQuery query;


    if(this->isMember(m_proxyConnection->session()->value("logged").toUInt(), group_id.toUInt())){

        query.prepare("SELECT * FROM users"
                      "INNER JOIN group_users ON users.id = group_users.user_id"
                      "WHERE group_users.status = 1 AND group_users.group_id = :group_id");
        query.bindValue(":group_id", group_id);
        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        while(query.next()){

           QVariantMap user;
           user.insert("first_name",query.value(query.record().indexOf("first_name")));
           user.insert("last_name",query.value(query.record().indexOf("last_name")));
           if(this->isAdmin(query.value(query.record().indexOf("id")).toUInt(),group_id.toUInt()))
               user.insert("isAdmin","1");
           else
               user.insert("isAdmin","0");

           users.append(user);
        }
        return req->response(QVariant(users),IResponse::OK);
    }
    return req->response(IResponse::FORBIDDEN);

}

IResponse *GroupsService::getUsersGroups( IRequest *req)
{
    QVariantList groups;
    QSqlQuery query;

    if(m_proxyConnection->session()->value("logged") !=""){


        query.prepare("SELECT * FROM groups"
                      "INNER JOIN group_users ON groups.id = group_users.group_id"
                      "WHERE group_users.status = 1 AND group_users.user_id = :user_id");
        query.bindValue(":user_id", m_proxyConnection->session()->value("logged").toInt());
        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        while(query.next()){

           QVariantMap group;
           group.insert("first_name",query.value(query.record().indexOf("user_id")));
           group.insert("lastn_name",query.value(query.record().indexOf("last_name")));

           groups.append(group);
        }

        return req->response(groups,IResponse::OK);
    }
    return req->response(IResponse::FORBIDDEN);


}

IResponse *GroupsService::getGroupTypes(IRequest *req)
{
    QVariantList types;
    QSqlQuery query;

    if(m_proxyConnection->session()->value("logged") !=""){


        query.prepare("SELECT * FROM group_types");
        if(!query.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);

        while(query.next()){

           QVariantMap type;
           type.insert("id",query.value(query.record().indexOf("id")));
           type.insert("name",query.value(query.record().indexOf("name")));

           types.append(type);
        }

        return req->response(types,IResponse::OK);
    }
    return req->response(IResponse::FORBIDDEN);


}

IResponse *GroupsService::deleteUser( IRequest *req)
{

    QVariantMap reqJson = req->postBodyFromJson().toMap();

    QSqlQuery query1;
    bool allowDelete = false;

    QString curUser_id = m_proxyConnection->session()->value("logged").toString();

    if(curUser_id == reqJson["user_id"] )
        allowDelete = true;
    else{
        if(this->isAdmin(curUser_id.toUInt(), reqJson["group_id"].toUInt()))
          allowDelete = true;
    }
    if(allowDelete){

        query1.prepare("SELECT * FROM group_users WHERE user_id = :user_id AND group_id = :group_id");
        query1.bindValue(":user_id", reqJson["user_id"]);
        query1.bindValue(":group_id", reqJson["group_id"]);
        if(query1.exec())
            return req->response(IResponse::INTERNAL_SERVER_ERROR);
        if(query1.first()){

            IDatabaseUpdate *update = m_proxyConnection->databaseUpdate();

            IDatabaseUpdateQuery *query = update->createUpdateQuery("group_users", IDatabaseUpdateQuery::Delete);
            query->setUpdateDates(true);
            query->setWhere("user_id", reqJson["user_id"]);
            query->setWhere("group_id",reqJson["group_id"]);
            if(update->execute())
                    req->response(IResponse::INTERNAL_SERVER_ERROR);

           return req->response(IResponse::OK);

        }
        else{
            return req->response(IResponse::BAD_REQUEST);
        }
    }
    else{
        return req->response(IResponse::FORBIDDEN);
    }

}
