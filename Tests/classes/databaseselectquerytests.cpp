#include "databaseselectquerytests.h"

#include "stubdatabase.h"
#include "databaseupdatequery.h"
#include "databaseselectquery.h"
#include "idatabaseselectquerywheregroup.h"

#include <QTest>
#include <QSqlQuery>

DatabaseSelectQueryTests::DatabaseSelectQueryTests(QObject *parent) :
    QObject(parent)
{
}

void DatabaseSelectQueryTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
    QSqlQuery query("CREATE TABLE tst_settings (_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,key TEXT NOT NULL,value TEXT NOT NULL,date_created TEXT NOT NULL,date_updated TEXT NOT NULL,uid TEXT);");
    query.exec();
}

void DatabaseSelectQueryTests::cleanupTestCase()
{
    StubDatabase::close();
}

void DatabaseSelectQueryTests::testOrderBy()
{
    {
        DatabaseUpdateQuery insert("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);
        insert.setColumnValue("key", "Peter");
        insert.setColumnValue("value", "Brown");
        insert.setUpdateDates(true);
        insert.executeQuery();
    }
    {
        DatabaseUpdateQuery insert("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);
        insert.setColumnValue("key", "Joseph");
        insert.setColumnValue("value", "Zuckerberg");
        insert.setUpdateDates(true);
        insert.executeQuery();
    }
    {
        DatabaseUpdateQuery insert("tst_settings", DatabaseUpdateQuery::InsertOrUpdate);
        insert.setColumnValue("key", "Xenia");
        insert.setColumnValue("value", "Zuckerberg");
        insert.setUpdateDates(true);
        insert.executeQuery();
    }
    DatabaseSelectQuery select("tst_settings");
    select.orderBy("value");
    select.orderBy("key", DatabaseSelectQuery::Descending);

    Q_ASSERT(select.next());
    QCOMPARE(select.value("value").toString(), QString("Brown"));
    QCOMPARE(select.value("key").toString(), QString("Peter"));

    Q_ASSERT(select.next());
    QCOMPARE(select.value("value").toString(), QString("Zuckerberg"));
    QCOMPARE(select.value("key").toString(), QString("Xenia"));

    Q_ASSERT(select.next());
    QCOMPARE(select.value("value").toString(), QString("Zuckerberg"));
    QCOMPARE(select.value("key").toString(), QString("Joseph"));
}

void DatabaseSelectQueryTests::testColumnsUsedInWhere()
{
    {
        DatabaseUpdateQuery query("tst_settings");
        query.setColumnValue("key", "John");
        query.setColumnValue("value", "Siracusa");
        query.setUpdateDates(true);
        query.executeQuery();
    }
    {
        DatabaseUpdateQuery query("tst_settings");
        query.setType(IDatabaseUpdateQuery::Delete);
        query.singleWhere("key", "John");
        query.executeQuery();
        QVariantMap columns = query.columnsForListeners().first().toMap();
        QCOMPARE(columns.value("key").toString(), QString("John"));
        QCOMPARE(columns.value("value").toString(), QString("Siracusa"));
    }
    {
        DatabaseUpdateQuery query("tst_settings");
        query.setColumnValue("key", "John");
        query.setColumnValue("value", "Siracusa");
        query.setUpdateDates(true);
        query.executeQuery();
    }
    {
        DatabaseUpdateQuery query("tst_settings");
        query.singleWhere("key", "John");
        query.setColumnValue("value", "Brown");
        query.executeQuery();
        QVariantMap columns = query.columnsForListeners().first().toMap();
        QCOMPARE(columns.value("key").toString(), QString("John"));
        QCOMPARE(columns.value("value").toString(), QString("Brown"));
    }
}
