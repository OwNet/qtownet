#include "databaseinitializer.h"
#include "messagehelper.h"

#include <QtCore/QVariant>

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

    // TODO: Close?
}
