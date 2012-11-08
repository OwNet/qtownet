#include "usermodule.h"
#include <QSqlQuery>
#include <QString>

UserModule::UserModule(QObject *parent) :
    IModule(parent)
{
    setUrl("user");
}

/**
 * @brief UserModule::registerUser registers new user
 * @return bool
 */
bool UserModule::registerUser()
{
    QSqlQuery q;

    QSqlQuery query;

    query.prepare("INSERT INTO employee (id, name, salary) "
                  "VALUES (:id, :name, :salary)");
    query.bindValue(":id", 1001);
    query.bindValue(":name", "Thad Beaumont");
    query.bindValue(":salary", 65000);
    query.exec();

    if(q.exec())
        return true;
    else
        return false;
}


QByteArray* UserModule::processRequest(IBus *bus, ProxyRequest *req)
{
    //insert case which will call all the methods
}
