#include "applicationdatastorage.h"

#include "applicationenvironment.h"
#include "messagehelper.h"
#include "settings.h"

#include <QDir>

ApplicationDataStorage::ApplicationDataStorage(QObject *parent) :
    QObject(parent)
{
}

QDir ApplicationDataStorage::appDataDirectory() const
{
    QDir dir;
    QString path = Settings().value("application/data_folder_path").toString();
    if (!path.isEmpty()) {
        dir.setPath(path);
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());
        return dir;
    }
    return dir;
}

QDir ApplicationDataStorage::appResourcesDirectory() const
{
    QDir dir;
    QString path = Settings().value("application/resources_folder_path").toString();
    if (!path.isEmpty()) {
        dir.setPath(path);
        if (!dir.exists())
            dir.mkpath(dir.absolutePath());
        return dir;
    }
    return dir;
}

QString ApplicationDataStorage::databaseFilePath(const QString &workspaceId) const
{
    return appDataDirectory().absoluteFilePath(QString("%1.sqlite").arg(workspaceId));
}
