#include "workspaceinitializer.h"

#include "workspacehelper.h"

WorkspaceInitializer::WorkspaceInitializer(QObject *parent) :
    QObject(parent)
{
}

void WorkspaceInitializer::init()
{
    WorkspaceHelper::createCurrentWorkspaceIfDoesntExist();
}
