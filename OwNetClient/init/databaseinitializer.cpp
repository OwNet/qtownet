#include "databaseinitializer.h"
#include "messagehelper.h"

#include <QtCore/QVariant>
#include <QDebug>



DatabaseInitializer::DatabaseInitializer()
{
}


void DatabaseInitializer::init()
{
    // init application database

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ownet.sqlite");


    if (! db.open()) {
        MessageHelper::error(QObject::tr("Failed to open database"),
                                QObject::tr("Unable to open the database: %1.")
                             .arg(db.lastError().text()));

    }


    //Drop User table
    dropUserTable();

    //User Table init
    createUserTable();





    // TODO: Close?
}

void DatabaseInitializer::createUserTable()
{
     QSqlQuery q;

     //TO DO discuss types

     //LastVisit Registered LastLogin (Unix Time, the number of seconds since 1970-01-01 00:00:00 UTC)

     if( q.exec("create table Users (Id INTEGER PRIMARY KEY,"\
            "Firstname TEXT NOT NULL,"\
            "Lastname TEXT NOT NULL,"\
            "Role TEXT NOT NULL,"\
            "Password TEXT NOT NULL,"\
            "Email TEXT,"\
            "LastVisit INTEGER NOT NULL,"\
            "Registered INTEGER NOT NULL,"\
            "LastLogin INTEGER NOT NULL"\
            ");")
      ) qDebug() << "User Table Created";
     else
         qDebug() << "Failed to Create User table";

}

void DatabaseInitializer::dropUserTable()
{
    QSqlQuery q;
    q.exec("DROP TABLE Users");
}
