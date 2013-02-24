#include "workspaceinitializertests.h"

#include "stubdatabase.h"
#include "settings.h"
#include "workspaceinitializer.h"

#include <QtTest>

WorkspaceInitializerTests::WorkspaceInitializerTests(QObject *parent) :
    QObject(parent)
{
}

void WorkspaceInitializerTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
}

void WorkspaceInitializerTests::cleanupTestCase()
{
    StubDatabase::close();
}

void WorkspaceInitializerTests::testInit()
{
    Settings settings;
    settings.beginGroup("current_workspace");
    if (settings.contains("id"))
        settings.remove("id");

    WorkspaceInitializer initializer;
    initializer.init();

    QCOMPARE(settings.contains("id"), true);
    QString id = settings.value("id").toString();

    initializer.init();
    QCOMPARE(settings.value("id").toString(), id);
}
