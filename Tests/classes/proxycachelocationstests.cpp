#include "proxycachelocationstests.h"

#include "stubdatabase.h"
#include "cachelocations.h"
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
        CacheLocations locations;
        DatabaseSettings settings;
        locations.addLocation("WEB", QDateTime::currentDateTime());
        locations.addLocation(settings.clientId(), QDateTime::currentDateTime());
        locations.addLocation("CA0879", QDateTime::currentDateTime());

        QStringList sorted = locations.sortedLocations();
        QCOMPARE(sorted.at(0), settings.clientId());
        QCOMPARE(sorted.at(1), QString("CA0879"));
        QCOMPARE(sorted.at(2), QString("WEB"));
    }
    {
        CacheLocations locations;
        DatabaseSettings settings;
        QDateTime earlier = QDateTime::currentDateTime().addSecs(-4000);

        locations.addLocation("WEB", QDateTime::currentDateTime());
        locations.addLocation(settings.clientId(), earlier);
        locations.addLocation("CA0879", earlier);

        QStringList sorted = locations.sortedLocations();
        QCOMPARE(sorted.at(0), QString("WEB"));
        QCOMPARE(sorted.at(1), settings.clientId());
        QCOMPARE(sorted.at(2), QString("CA0879"));
    }
}
