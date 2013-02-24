#include "workspacehelper.h"

#include "settings.h"
#include "uniqueidhelper.h"
#ifndef TEST
#include <QApplication>
#include <QProcess>
#endif

WorkspaceHelper::WorkspaceHelper(QObject *parent) :
    QObject(parent)
{
}

QString WorkspaceHelper::currentWorkspaceId()
{
    Settings settings;
    settings.beginGroup("current_workspace");
    return settings.value("id").toString();
}

QString WorkspaceHelper::currentWorkspaceName()
{
    Settings settings;
    settings.beginGroup("current_workspace");
    return settings.value("name").toString();
}

/**
 * @brief Change the name of the current workspace
 * @param name New workspace name
 */
void WorkspaceHelper::setCurrentWorkspaceName(const QString &name)
{
    Settings settings;
    settings.beginGroup("current_workspace");
    settings.setValue("name", name);
    QString id = settings.value("id").toString();
    settings.endGroup();

    settings.beginGroup("workspaces");
    settings.setValue(id, name);
}

/**
 * @brief Initializes the ID and default name for the workspace if it doesn't already exist.
 * Called on startup by WorkspaceInitializer.
 */
void WorkspaceHelper::createCurrentWorkspaceIfDoesntExist()
{
    Settings settings;
    settings.beginGroup("current_workspace");
    QString name = tr("OwNet Network");
    if (!settings.contains("id")) {
        QString id = UniqueIdHelper::generate();
        settings.setValue("id", id);
        settings.setValue("name", name);
        settings.endGroup();
        settings.beginGroup("workspaces");
        settings.setValue(id, name);
    }
}

/**
 * @brief Change the current workspace. This requires the app to be restarted and loaded
 * with a different database.
 * @param id ID of the workspace to be loaded
 * @param name Name of the workspace to be loaded
 */
void WorkspaceHelper::loadWorkspace(const QString &id, const QString &name)
{
    {
        Settings settings;
        settings.beginGroup("current_workspace");
        settings.setValue("id", id);
        settings.setValue("name", name);
    }
#ifndef TEST
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
#endif
}
