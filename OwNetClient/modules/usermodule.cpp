#include "usermodule.h"
#include <QSqlQuery>
#include <QString>
#include <QByteArray>
#include <QDebug>



UserModule::UserModule(QObject *parent) :
    IModule(parent)
{
    setUrl("users.js");


}


// create element
QByteArray* UserModule::create(IBus *bus, ProxyRequest *req)
{
}

// show element
QByteArray* UserModule::show( IBus *bus, ProxyRequest *req)
{
}

//delete element
QByteArray* UserModule::del( IBus *bus,  ProxyRequest *req)
{
}

//edit element
QByteArray* UserModule::edit( IBus *bus,  ProxyRequest *req)
{
}

QByteArray* UserModule::index( IBus *bus,  ProxyRequest *req)
{

    return new QByteArray();
}



/*QByteArray* UserModule::registerUser(IBus *bus, QByteArray data)
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
        bus->setHttpStatus(200, "OK");
    else
        bus->setHttpStatus(400,"Bad Request");


}

QByteArray* UserModule::getAllUsers(IBus *bus, QByteArray data)
{
}*/

