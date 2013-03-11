#include "proxycachelocationstests.h"

#include "stubdatabase.h"
#include "proxycachelocations.h"
#include "databasesettings.h"

#include <QtTest>

ProxyCacheLocationsTests::ProxyCacheLocationsTests(QObject *parent) :
    QObject(parent)
{
}

void ProxyCacheLocationsTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
}

void ProxyCacheLocationsTests::cleanupTestCase()
{
    StubDatabase::close();
}

void ProxyCacheLocationsTests::testSortedLocations()
{
    {
        ProxyCacheLocations locations;
        DatabaseSettings settings;
        locations.addLocation("WEB", QDateTime::currentDateTime());
        locations.addLocation(settings.clientId(), QDateTime::currentDateTime());
        locations.addLocation("CA0879", QDateTime::currentDateTime());

        QList<ProxyCacheLocation*> sorted = locations.sortedLocations();
        QCOMPARE(sorted.at(0)->clientId(), settings.clientId());
        QCOMPARE(sorted.at(1)->clientId(), QString("CA0879"));
        QCOMPARE(sorted.at(2)->clientId(), QString("WEB"));
    }
    {
        ProxyCacheLocations locations;
        DatabaseSettings settings;
        QDateTime earlier = QDateTime::currentDateTime().addSecs(-4000);

        locations.addLocation("WEB", QDateTime::currentDateTime());
        locations.addLocation(settings.clientId(), earlier);
        locations.addLocation("CA0879", earlier);

        QList<ProxyCacheLocation*> sorted = locations.sortedLocations();
        QCOMPARE(sorted.at(0)->clientId(), QString("WEB"));
        QCOMPARE(sorted.at(1)->clientId(), settings.clientId());
        QCOMPARE(sorted.at(2)->clientId(), QString("CA0879"));
    }
}
