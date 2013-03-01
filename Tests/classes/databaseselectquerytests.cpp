#include "databaseselectquerytests.h"

#include "stubdatabase.h"
#include "databaseupdatequery.h"
#include "databaseselectquery.h"

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
    QSqlQuery query("CREATE TABLE tst_settings (_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,key TEXT NOT NULL,value TEXT NOT NULL,date_created TEXT NOT NULL,date_updated TEXT NOT NULL,sync_id TEXT);");
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
