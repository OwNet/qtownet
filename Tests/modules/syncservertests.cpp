#include "syncservertests.h"

#include <QtTest>

#include "stubdatabase.h"
#include "syncserver.h"
#include "proxyconnection.h"
#include "synceddatabaseupdatequery.h"
#include "databasesettings.h"
#include "databaseselectquery.h"
#include "databaseupdate.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>

SyncServerTests::SyncServerTests(QObject *parent) :
    QObject(parent)
{
}

void SyncServerTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
    DatabaseSettings().clearCache();
    QSqlQuery query("CREATE TABLE tst_settings (_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,key TEXT NOT NULL,value TEXT NOT NULL,sync_id TEXT NOT NULL);");
    query.exec();
}

void SyncServerTests::cleanupTestCase()
{
    StubDatabase::close();
}

void SyncServerTests::testClientRecordNumbers()
{
    ProxyConnection proxyConnection;
    SyncServer server(&proxyConnection);
    QVariantMap groupsMap = server.clientRecordNumbers();
    QCOMPARE(groupsMap.count(), 0);

    {
        SyncedDatabaseUpdateQuery query("tst_settings");
        query.setColumnValue("key", "Just testing");
        query.setColumnValue("value", "No stress");
        query.executeQuery();
        DatabaseUpdate::saveLastRecordNumbers();
    }

    groupsMap = server.clientRecordNumbers();
    QCOMPARE(groupsMap.count(), 1);
    QVariantMap clientsMap = groupsMap.value(QString()).toMap();
    int value = clientsMap.value(QString::number(DatabaseSettings().clientId())).toInt();
    QCOMPARE(value, 0);

    {
        SyncedDatabaseUpdateQuery query("tst_settings");
        query.setColumnValue("key", "Still testing");
        query.setColumnValue("value", "No stress");
        query.setGroupId(1);
        query.executeQuery();
        DatabaseUpdate::saveLastRecordNumbers();
    }

    groupsMap = server.clientRecordNumbers();
    QCOMPARE(groupsMap.count(), 2);
    clientsMap = groupsMap.value("1").toMap();
    value = clientsMap.value(QString::number(DatabaseSettings().clientId())).toInt();
    QCOMPARE(value, 1);
}

void SyncServerTests::testUpdates()
{
    ProxyConnection proxyConnection;
    SyncServer server(&proxyConnection);
    QCOMPARE(server.updates(QVariantMap(), true, -1).count(), 0);

    {
        SyncedDatabaseUpdateQuery query("tst_settings");
        query.setColumnValue("key", "Just testing");
        query.setColumnValue("value", "No stress");
        query.executeQuery();
        DatabaseUpdate::saveLastRecordNumbers();
    }
    QCOMPARE(server.updates(QVariantMap(), true, -1).count(), 1);

    {
        SyncedDatabaseUpdateQuery query("tst_settings");
        query.setColumnValue("value", "Stress");
        query.singleWhere("key", "Just testing");
        query.executeQuery();
        DatabaseUpdate::saveLastRecordNumbers();
    }

    int clientId1 = DatabaseSettings().clientId();
    QVariantList updates1 = server.updates(QVariantMap(), true, -1);
    QVariantMap clientRecordNumbers1 = server.clientRecordNumbers();
    QCOMPARE(updates1.count(), 2);

    init();

    int clientId2 = DatabaseSettings().clientId();
    QVERIFY2(clientId1 != clientId2, "Client IDs are the same");

    {
        SyncedDatabaseUpdateQuery query("tst_settings", IDatabaseUpdateQuery::InsertOrUpdate);
        query.setColumnValue("key", "Another test");
        query.setColumnValue("value", "Went well");
        query.singleWhere("key", "Another test");
        query.executeQuery();
    }
    {
        SyncedDatabaseUpdateQuery query("tst_settings");
        query.setColumnValue("key", "Yet another test");
        query.setColumnValue("value", "Went well");
        query.setGroupId(1);
        query.executeQuery();
        DatabaseUpdate::saveLastRecordNumbers();
    }
    QCOMPARE(server.updates(clientRecordNumbers1, true, clientId1).count(), 2);
    QCOMPARE(server.updates(clientRecordNumbers1, false, clientId1).count(), 1);

    QVariantMap clientRecordNumbers2 = server.clientRecordNumbers();
    server.saveAndApplyUpdates(updates1);

    /// Check if all changes were applied
    DatabaseSelectQuery selectQuery("tst_settings");
    selectQuery.select("COUNT(*) AS count");
    selectQuery.first();
    QCOMPARE(selectQuery.value("count").toInt(), 3);

    QCOMPARE(server.updates(clientRecordNumbers2, true, -1).count(), 2);

    {
        DatabaseSelectQuery selectQuery("tst_settings");
        selectQuery.singleWhere("key", "Just testing");
        QCOMPARE(selectQuery.first(), true);
        QCOMPARE(selectQuery.value("value").toString(), QString("Stress"));
        QCOMPARE(server.updates(QVariantMap(), true, -1).count(), 4);
    }

    clientRecordNumbers2 = server.clientRecordNumbers();
    QVariantList updates2 = server.updates(clientRecordNumbers1, true, clientId1);

    init();

    server.saveAndApplyUpdates(updates1);
    server.saveAndApplyUpdates(updates2);
    //StubDatabase::dumpTable("sync_journal");

    QCOMPARE(server.updates(clientRecordNumbers2, true, clientId2).count(), 0);

    {
        DatabaseSelectQuery selectQuery("tst_settings");
        selectQuery.singleWhere("key", "Yet another test");
        QCOMPARE(selectQuery.first(), true);
        QCOMPARE(selectQuery.value("value").toString(), QString("Went well"));
        QCOMPARE(server.updates(QVariantMap(), true, -1).count(), 4);
    }
}
