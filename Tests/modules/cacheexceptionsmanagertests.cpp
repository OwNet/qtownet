#include "cacheexceptionsmanagertests.h"

#include "cacheexceptionsmanager.h"
#include "stubdatabase.h"

#include <QtTest>

CacheExceptionsManagerTests::CacheExceptionsManagerTests(QObject *parent) :
    QObject(parent)
{
}

void CacheExceptionsManagerTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
}

void CacheExceptionsManagerTests::cleanupTestCase()
{
    StubDatabase::close();
}

void CacheExceptionsManagerTests::testGetHostName()
{
    QCOMPARE(CacheExceptionsManager::getHostName("http://www.sme.sk"), QString("sme.sk"));
    QCOMPARE(CacheExceptionsManager::getHostName("http://auto.sme.sk"), QString("sme.sk"));
    QCOMPARE(CacheExceptionsManager::getHostName("http://auto.sme.sk/novy_cas?source=topky"), QString("sme.sk"));
}
