#include "syncservertests.h"

#include <QtTest>

#include "stubdatabase.h"
#include "syncserver.h"
#include "proxyconnection.h"
#include "databaseupdate.h"
#include "databasesettings.h"
#include "databaseselectquery.h"

SyncServerTests::SyncServerTests(QObject *parent) :
    QObject(parent)
{
}

void SyncServerTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
    DatabaseSettings().clearCache();
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
        DatabaseUpdate update;
        IDatabaseUpdateQuery *query = update.createUpdateQuery("settings");
        query->setColumnValue("key", "Just testing");
        query->setColumnValue("value", "No stress");
        update.execute();
        update.saveLastRecordNumbers();
    }

    groupsMap = server.clientRecordNumbers();
    QCOMPARE(groupsMap.count(), 1);
    QVariantMap clientsMap = groupsMap.value(QString()).toMap();
    int value = clientsMap.value(QString::number(DatabaseSettings().clientId())).toInt();
    QCOMPARE(value, 0);

    {
        DatabaseUpdate update;
        IDatabaseUpdateQuery *query = update.createUpdateQuery("settings");
        query->setColumnValue("key", "Still testing");
        query->setColumnValue("value", "No stress");
        update.setGroupId(1);
        update.execute();
        update.saveLastRecordNumbers();
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
        DatabaseUpdate update;
        IDatabaseUpdateQuery *query = update.createUpdateQuery("settings");
        query->setColumnValue("key", "Just testing");
        query->setColumnValue("value", "No stress");
        update.execute();
        update.saveLastRecordNumbers();
    }
    QCOMPARE(server.updates(QVariantMap(), true, -1).count(), 1);

    {
        DatabaseUpdate update;
        IDatabaseUpdateQuery *query = update.createUpdateQuery("settings", IDatabaseUpdateQuery::Detect);
        query->setColumnValue("value", "Stress");
        query->setWhere("key", "Just testing");
        update.execute();
        update.saveLastRecordNumbers();
    }

    int clientId1 = DatabaseSettings().clientId();
    QVariantList updates1 = server.updates(QVariantMap(), true, -1);
    QVariantMap clientRecordNumbers1 = server.clientRecordNumbers();
    QCOMPARE(updates1.count(), 2);

    init();

    int clientId2 = DatabaseSettings().clientId();
    QVERIFY2(clientId1 != clientId2, "Client IDs are the same");

    {
        DatabaseUpdate update;
        IDatabaseUpdateQuery *query = update.createUpdateQuery("settings", IDatabaseUpdateQuery::Detect);
        query->setColumnValue("key", "Another test");
        query->setColumnValue("value", "Went well");
        query->setWhere("key", "Another test");
        update.execute();
    }
    {
        DatabaseUpdate update;
        IDatabaseUpdateQuery *query = update.createUpdateQuery("settings");
        query->setColumnValue("key", "Yet another test");
        query->setColumnValue("value", "Went well");
        update.setGroupId(1);
        update.execute();
        update.saveLastRecordNumbers();
    }
    QCOMPARE(server.updates(clientRecordNumbers1, true, clientId1).count(), 2);
    QCOMPARE(server.updates(clientRecordNumbers1, false, clientId1).count(), 1);

    QVariantMap clientRecordNumbers2 = server.clientRecordNumbers();
    server.saveAndApplyUpdates(updates1);
    QCOMPARE(server.updates(clientRecordNumbers2, true, -1).count(), 2);

    {
        DatabaseSelectQuery selectQuery("settings");
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
    QCOMPARE(server.updates(clientRecordNumbers2, true, clientId2).count(), 0);

    {
        DatabaseSelectQuery selectQuery("settings");
        selectQuery.singleWhere("key", "Yet another test");
        QCOMPARE(selectQuery.first(), true);
        QCOMPARE(selectQuery.value("value").toString(), QString("Went well"));
        QCOMPARE(server.updates(QVariantMap(), true, -1).count(), 4);
    }
}
