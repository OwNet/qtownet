#include "databaseinitializer.h"
#include "messagehelper.h"

#include "applicationdatastorage.h"
#include "applicationenvironment.h"
#include "proxydownloads.h"

#include <QtCore/QVariant>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>
#include <QCoreApplication>

DatabaseInitializer::DatabaseInitializer()
{
}

void DatabaseInitializer::init()
{
    openDatabase();
    runMigrations();
}

/**
 * @brief Opens singleton application database.
 */
void DatabaseInitializer::openDatabase()
{
    // configure stub database name
    QString databaseName = ApplicationEnvironment().value("OWNET_DATABASE_NAME", "ownet.sqlite");

    MessageHelper::debug(QObject::tr("Opening database %1")
                         .arg(databaseName));

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);

    if (!db.open()) {
        MessageHelper::error(QObject::tr("Failed to open database"),
                                QObject::tr("Unable to open the database: %1.")
                             .arg(db.lastError().text()));
    }
}

/**
 * @brief Runs all new migrations from migrations directory.
 */
void DatabaseInitializer::runMigrations()
{
    // migrations table
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS migrations (name TEXT PRIMARY KEY); ");

    // in migrations directory
    QDir appDataDir = ApplicationDataStorage().appDataDirectory();
    if (appDataDir.exists("migrations")) {
        QDir migrationsDir(appDataDir.absolutePath().append("/migrations"));

        QFileInfoList list = migrationsDir.entryInfoList(QDir::Files, QDir::Name);

        // for all migrations
        for (int i = 0; i < list.size(); ++i) {
            QSqlQuery query;
            query.prepare("SELECT name FROM migrations WHERE name = ?; ");
            query.addBindValue(list.at(i).fileName());
            query.exec();

            // unless migration has been run
            if (! query.next())
            {
                MessageHelper::debug(QObject::tr("Running migration %1.")
                                     .arg(list.at(i).fileName()));

                // read file
                QFile file(list.at(i).absoluteFilePath());
                if(file.open(QIODevice::ReadOnly)) {
                    QTextStream in(&file);

                    // execute each line
                    QSqlQuery query;
                    while (!in.atEnd()) {
                        QString line = in.readLine();
                        query.exec(line);
                    }

                    file.close();
                }
                else {
                    MessageHelper::debug(QObject::tr("%1: %2.")
                                         .arg(file.errorString())
                                         .arg(list.at(i).fileName()));
                }

                // add to migrations table
                QSqlQuery query;
                query.prepare("INSERT INTO migrations (name) VALUES (?); ");
                query.addBindValue(list.at(i).fileName());
                query.exec();
            }
        }
    }
}
