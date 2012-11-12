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
    dropUsersTable();

    //User Table init
    createUsersTable();





    // TODO: Close?
}

void DatabaseInitializer::createUsersTable()
{
     QSqlQuery q;

     //TO DO discuss types

     //LastVisit Registered LastLogin (Unix Time, the number of seconds since 1970-01-01 00:00:00 UTC)
     /*"last_visit INTEGER NOT NULL,"\
     "registered INTEGER NOT NULL,"\
     "last_login INTEGER NOT NULL"\

      pridat neskor
      */

     if( q.exec("create table users (id INTEGER PRIMARY KEY,"\
            "first_name TEXT NOT NULL,"\
            "last_name TEXT NOT NULL,"\
            "login TEXT NOT NULL,"\
            "role TEXT NOT NULL,"\
            "password TEXT NOT NULL,"\
            "email TEXT"\
            ");")
      ) qDebug() << "User Table Created";
     else
         qDebug() << "Failed to Create User table";

}

void DatabaseInitializer::dropUsersTable()
{
    QSqlQuery q;
    q.exec("DROP TABLE Users");
}
