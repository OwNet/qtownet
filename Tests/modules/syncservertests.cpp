#include "syncservertests.h"

#include <QtTest>

#include "stubdatabase.h"
#include "syncserver.h"
#include "proxyconnection.h"
#include "databaseupdate.h"
#include "databasesettings.h"

SyncServerTests::SyncServerTests(QObject *parent) :
    QObject(parent)
{
}

void SyncServerTests::initTestCase()
{
    StubDatabase::init();
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
