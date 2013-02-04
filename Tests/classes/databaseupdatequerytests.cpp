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
    QSqlQuery query("CREATE TABLE tst_settings (_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,key TEXT NOT NULL,value TEXT NOT NULL,date_created TEXT NOT NULL,date_updated TEXT NOT NULL,sync_id TEXT);");
    query.exec();
}

void DatabaseUpdateQueryTests::cleanupTestCase()
{
    StubDatabase::close();
}

void DatabaseUpdateQueryTests::testInsert()
{
    {
        SyncedDatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);

        query.setColumnValue("key", "John");
        query.setColumnValue("value", "Doe");
        query.setUpdateDates(true);

        query.executeQuery();
        DatabaseUpdate::saveLastRecordNumbers();

        /// Check that the insert was successful
        DatabaseSelectQuery select("tst_settings");
        QCOMPARE(select.first(), true);
        QCOMPARE(select.value("key").toString(), QString("John"));
        QCOMPARE(select.value("value").toString(), QString("Doe"));

        DatabaseSettings settings;

        /// Check that the inserted item is in the sync journal
        DatabaseSelectQuery selectJournal("sync_journal");
        QCOMPARE(selectJournal.first(), true);
        QCOMPARE(selectJournal.value("client_id").toUInt(), settings.clientId());
        QCOMPARE(selectJournal.value("table_name").toString(), QString("tst_settings"));
        QCOMPARE(selectJournal.value("sync_id").toString(), QString("%1/%2").arg(settings.clientId())
                 .arg(selectJournal.value("client_rec_num").toInt()));
    }
    {
        DatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);

        query.setColumnValue("key", "Peter");
        query.setColumnValue("value", "Brown");
        query.setUpdateDates(true);

        query.executeQuery();
        DatabaseUpdate::saveLastRecordNumbers();

        /// Check that the insert was successful
        DatabaseSelectQuery select("tst_settings");
        select.singleWhere("value", "Brown");
        QCOMPARE(select.first(), true);
        QCOMPARE(select.value("key").toString(), QString("Peter"));
        QCOMPARE(select.value("value").toString(), QString("Brown"));

        /// Check that the inserted item is not in the sync journal
        DatabaseSelectQuery selectJournal("sync_journal");
        selectJournal.select("COUNT(*) AS count");
        QCOMPARE(selectJournal.first(), true);
        QCOMPARE(selectJournal.value("count").toInt(), 1);
    }
}

void DatabaseUpdateQueryTests::testUpdate()
{
    {
        {
            SyncedDatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);
            query.setColumnValue("key", "John");
            query.setColumnValue("value", "Doe");
            query.setUpdateDates(true);
            query.executeQuery();
        }
        {
            SyncedDatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);
            query.setColumnValue("key", "Joseph");
            query.singleWhere("value", "Doe");
            query.setUpdateDates(true);
            query.executeQuery();
        }
        DatabaseUpdate::saveLastRecordNumbers();

        /// Check if the update was succesful
        DatabaseSelectQuery select("tst_settings");
        QCOMPARE(select.first(), true);
        QCOMPARE(select.value("key").toString(), QString("Joseph"));
        QCOMPARE(select.value("value").toString(), QString("Doe"));

        /// Count tst_settings records - should be just one
        DatabaseSelectQuery selectCount("tst_settings");
        selectCount.select("COUNT(*) AS count");
        QCOMPARE(selectCount.first(), true);
        QCOMPARE(selectCount.value("count").toInt(), 1);

        /// Check if both insert and update are in the sync journal
        DatabaseSelectQuery selectJournal("sync_journal");
        selectJournal.select("COUNT(*) AS count");
        QCOMPARE(selectJournal.first(), true);
        QCOMPARE(selectJournal.value("count").toInt(), 2);
    }
    {
        {
            DatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);
            query.setColumnValue("key", "Peter");
            query.singleWhere("value", "Doe");
            query.setUpdateDates(true);
            query.executeQuery();
        }
        DatabaseUpdate::saveLastRecordNumbers();

        /// Check if the update was succesful
        DatabaseSelectQuery select("tst_settings");
        QCOMPARE(select.first(), true);
        QCOMPARE(select.value("key").toString(), QString("Peter"));
        QCOMPARE(select.value("value").toString(), QString("Doe"));

        /// Count tst_settings records - should be just one
        DatabaseSelectQuery selectCount("tst_settings");
        selectCount.select("COUNT(*) AS count");
        QCOMPARE(selectCount.first(), true);
        QCOMPARE(selectCount.value("count").toInt(), 1);

        /// Check that the update is not in the sync journal
        DatabaseSelectQuery selectJournal("sync_journal");
        selectJournal.select("COUNT(*) AS count");
        QCOMPARE(selectJournal.first(), true);
        QCOMPARE(selectJournal.value("count").toInt(), 2);
    }
}

void DatabaseUpdateQueryTests::testDelete()
{
    {
        {
            SyncedDatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);
            query.setColumnValue("key", "John");
            query.setColumnValue("value", "Doe");
            query.setUpdateDates(true);
            query.executeQuery();
        }
        {
            SyncedDatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::Delete);
            query.singleWhere("value", "Doe");
            query.setUpdateDates(true);
            query.executeQuery();
        }
        DatabaseUpdate::saveLastRecordNumbers();

        /// Check if the delete was succesful
        DatabaseSelectQuery select("tst_settings");
        QCOMPARE(select.first(), false);

        /// Check if both insert and delete are in the sync journal
        DatabaseSelectQuery selectJournal("sync_journal");
        selectJournal.select("COUNT(*) AS count");
        QCOMPARE(selectJournal.first(), true);
        QCOMPARE(selectJournal.value("count").toInt(), 2);
    }
    {
        {
            DatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);
            query.setColumnValue("key", "John");
            query.setColumnValue("value", "Doe");
            query.setUpdateDates(true);
            query.executeQuery();
        }
        {
            DatabaseUpdateQuery query("tst_settings", DatabaseUpdateQuery::Delete);
            query.singleWhere("value", "Doe");
            query.setUpdateDates(true);
            query.executeQuery();
        }
        DatabaseUpdate::saveLastRecordNumbers();

        /// Check if the delete was succesful
        DatabaseSelectQuery select("tst_settings");
        QCOMPARE(select.first(), false);

        /// Check that last queries were not added to the sync journal
        DatabaseSelectQuery selectJournal("sync_journal");
        selectJournal.select("COUNT(*) AS count");
        QCOMPARE(selectJournal.first(), true);
        QCOMPARE(selectJournal.value("count").toInt(), 2);
    }
}
