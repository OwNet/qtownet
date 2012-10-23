#include "databaseinitializer.h"
#include "messagehelper.h"

#include <QtCore/QVariant>
#include <QSqlQuery>

DatabaseInitializer::DatabaseInitializer()
{
}

void DatabaseInitializer::init()
{
    // init application database

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ownet.sqlite");

    if (! db.open())
    {
        MessageHelper::error(QObject::tr("Failed to open database"),
                                QObject::tr("Unable to open the database: %1.")
                             .arg(db.lastError().text()));
    }

    // TODO: Close

    // Anywhere in the proxy (example)

    MessageHelper::debug("Querying...");

    QSqlQuery q;
    q.exec("create table pages (id INTEGER PRIMARY KEY,url TEXT);");
    q.exec("insert into pages (url) values (\"http://www.qt.com\");");
    q.exec("SELECT * FROM pages;");
    while (q.next()) {
        MessageHelper::debug(QObject::tr("%1 %2").arg(q.value(0).toString()).arg(q.value(1).toString()));
    }
}
