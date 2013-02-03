#include "databaseupdatequerytests.h"

#include "stubdatabase.h"
#include "synceddatabaseupdatequery.h"
#include "databaseselectquery.h"
#include "databasesettings.h"
#include "databaseupdate.h"

#include <QtTest>
#include <QSqlQuery>

DatabaseUpdateQueryTests::DatabaseUpdateQueryTests(QObject *parent) :
    QObject(parent)
{
}

void DatabaseUpdateQueryTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
}

void DatabaseUpdateQueryTests::cleanupTestCase()
{
    StubDatabase::close();
}

void DatabaseUpdateQueryTests::testExecuteQuery()
{
    SyncedDatabaseUpdateQuery query("users", DatabaseUpdateQuery::InsertOrUpdate);

    query.setColumnValue("first_name", "John");
    query.setColumnValue("last_name", "Doe");
    query.setColumnValue("login", "john.doe");
    query.setColumnValue("role_id", 1);
    query.setColumnValue("password", "password");
    query.setColumnValue("salt", "salt");
    query.setColumnValue("email", "doe@mail.com");
    query.setUpdateDates(true);

    query.executeQuery();
    DatabaseUpdate::saveLastRecordNumbers();

    DatabaseSelectQuery selectUsers("users");
    QCOMPARE(selectUsers.first(), true);
    QCOMPARE(selectUsers.value("first_name").toString(), QString("John"));
    QCOMPARE(selectUsers.value("last_name").toString(), QString("Doe"));

    DatabaseSettings settings;

    DatabaseSelectQuery selectJournal("sync_journal");
    QCOMPARE(selectJournal.first(), true);
    QCOMPARE(selectJournal.value("client_id").toUInt(), settings.clientId());
    QCOMPARE(selectJournal.value("table_name").toString(), QString("users"));
    QCOMPARE(selectJournal.value("sync_id").toString(), QString("%1/%2").arg(settings.clientId())
             .arg(selectJournal.value("client_rec_num").toInt()));
}
