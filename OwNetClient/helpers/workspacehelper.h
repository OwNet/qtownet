#ifndef WORKSPACEHELPER_H
#define WORKSPACEHELPER_H

#include <QObject>

class WorkspaceHelper : public QObject
{
    Q_OBJECT
public:
    explicit WorkspaceHelper(QObject *parent = 0);

    static QString currentWorkspaceId();
    static QString currentWorkspaceName();
    static void setCurrentWorkspaceName(const QString &name);

    static void createCurrentWorkspaceIfDoesntExist();
    static void createAndLoadNewWorkspace(const QString &name);

    static void loadWorkspace(const QString &id, const QString &name);
    static bool removeWorkspace(const QString &id);
};

#endif // WORKSPACEHELPER_H
