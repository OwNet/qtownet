#include "workspacehelpertests.h"

#include "stubdatabase.h"
#include "settings.h"
#include "workspacehelper.h"

#include <QtTest>

WorkspaceHelperTests::WorkspaceHelperTests(QObject *parent) :
    QObject(parent)
{
}

void WorkspaceHelperTests::init()
{
    StubDatabase::close();
    StubDatabase::init();
}

void WorkspaceHelperTests::cleanupTestCase()
{
    StubDatabase::close();
}

void WorkspaceHelperTests::testCurrentWorkspace()
{
    Settings settings;
    settings.beginGroup("workspaces");
    int workspaceCount = settings.childKeys().count();
    WorkspaceHelper::createCurrentWorkspaceIfDoesntExist();
    QCOMPARE(settings.childKeys().count() - workspaceCount <= 1, true);
    workspaceCount = settings.childKeys().count();
    QCOMPARE(WorkspaceHelper::currentWorkspaceId().count() > 6, true);
    QCOMPARE(settings.childKeys().count() > 0, true);
    QCOMPARE(settings.value(WorkspaceHelper::currentWorkspaceId()).toString(), WorkspaceHelper::currentWorkspaceName());

    QString name = WorkspaceHelper::currentWorkspaceName();
    WorkspaceHelper::setCurrentWorkspaceName("WRKSPC");
    QCOMPARE(WorkspaceHelper::currentWorkspaceName(), QString("WRKSPC"));
    QCOMPARE(settings.value(WorkspaceHelper::currentWorkspaceId()).toString(), QString("WRKSPC"));
    WorkspaceHelper::setCurrentWorkspaceName(name);

    QCOMPARE(settings.childKeys().count() == workspaceCount, true);
}
