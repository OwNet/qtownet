#include "stubdatabase.h"

#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextStream>

#include "settings.h"
#include "messagehelper.h"
#include "applicationenvironment.h"
#include "applicationdatastorage.h"



void StubDatabase::init()
{    
    if ( QSqlDatabase::database().isOpen() ) {
        close();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open()) {
        MessageHelper::error(QObject::tr("Failed to open database"),
                             QObject::tr("Unable to open the database: %1.")
                             .arg(db.lastError().text()));
    }    

    db.exec("DROP ALL TABLES");
    runMigrations();
}

void StubDatabase::close()
{
    QSqlDatabase::database().close();
}

void StubDatabase::runMigrations()
{
    // set where QSettings will look for config file
    QCoreApplication::setOrganizationName("The Reconnected");
    QCoreApplication::setApplicationName("OwNet Client");


    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QMap<QString, bool> migrations;

    // migrations table
    query.exec("CREATE TABLE IF NOT EXISTS migrations (name TEXT PRIMARY KEY); ");

    // load migration names
    query.prepare("SELECT name FROM migrations; ");
    query.exec();
    while (query.next())
        migrations.insert(query.value(0).toString(), true);

    // in migrations directory
    QDir dir = ApplicationDataStorage().appResourcesDirectory();
    if (dir.exists("migrations")) {
        dir.cd("migrations");

        QFileInfoList list = dir.entryInfoList(QDir::Files, QDir::Name);

        // for all migrations
        for (int i = 0; i < list.size(); ++i) {

            // unless migration has been run
            bool success = true;
            if (! migrations.contains(list.at(i).fileName()))
            {
                //MessageHelper::debug(QObject::tr("Migration %1")
                //                     .arg(list.at(i).fileName()));

                // read file
                QFile file(list.at(i).absoluteFilePath());
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);
                    in.setCodec("UTF-8");
                    QStringList queries = in.readAll().split(";", QString::SkipEmptyParts);

                    db.transaction();

                    foreach (QString strQuery, queries) {
                        if (strQuery.trimmed().isEmpty())
                            continue;

                        QSqlQuery query;
                        if (!query.exec(strQuery)) {
                            MessageHelper::debug(query.lastError().text());
                            success = false;
                            break;
                        }
                    }

                    if (!success)
                        db.rollback();
                    else
                    {
                        // add to migrations table
                        QSqlQuery query;
                        query.prepare("INSERT INTO migrations (name) VALUES (?); ");
                        query.addBindValue(list.at(i).fileName());
                        query.exec();

                        db.commit();
                    }

                    file.close();
                }
                else {
                    MessageHelper::debug(file.errorString());
                }
            }

            if (! success)
                break;
        }
    }
}
