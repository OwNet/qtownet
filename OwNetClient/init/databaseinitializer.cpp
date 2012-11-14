#include "databaseinitializer.h"
#include "messagehelper.h"

#include "proxydownloads.h"

#include <QtCore/QVariant>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

DatabaseInitializer::DatabaseInitializer()
{
}

void DatabaseInitializer::init()
{
    // init application database

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ownet.sqlite");


    if (!db.open()) {
        MessageHelper::error(QObject::tr("Failed to open database"),
                                QObject::tr("Unable to open the database: %1.")
                             .arg(db.lastError().text()));
    }

    //User Table init
    createUserTable();

    // TODO: Close?
}

void DatabaseInitializer::createUserTable()
{
     QSqlQuery q;
     q.exec("create table Users (id INTEGER PRIMARY KEY,name TEXT);");
}
