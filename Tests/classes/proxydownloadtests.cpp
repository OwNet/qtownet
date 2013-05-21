#include "proxydownloadtests.h"

#include "stubdatabase.h"
#include "artificialrequest.h"
#include "proxyrequest.h"
#include "proxyhandlersession.h"

#include <QtTest>
#include <QFile>

ProxyDownloadTests::ProxyDownloadTests(QObject *parent) :
    QObject(parent)
{
}

void ProxyDownloadTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
}

void ProxyDownloadTests::cleanupTestCase()
{
    StubDatabase::close();
}

void ProxyDownloadTests::testInputAndOutput()
{
}

void ProxyDownloadTests::testSimultaneousReadAndWrite()
{
    QFile file("test.txt");
    file.open(QFile::WriteOnly);
    file.write("HAHA! This is funny!");
    file.flush();
    QFile fileRead("test.txt");
    fileRead.open(QFile::ReadOnly);
    QCOMPARE(fileRead.readAll(), QByteArray("HAHA! This is funny!"));
    file.write("Soooo funny!");
    file.flush();
    QCOMPARE(fileRead.readAll(), QByteArray("Soooo funny!"));
    file.remove();
}
