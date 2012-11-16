#include "applicationdatastorage.h"
#include "applicationenvironment.h"
#include "messagehelper.h"

#include <QDesktopServices>

ApplicationDataStorage::ApplicationDataStorage(QObject *parent) :
    QObject(parent)
{
}

QDir ApplicationDataStorage::appDataDirectory()
{
    // configure stub data storage
    if (ApplicationEnvironment().contains("OWNET_APP_DATA_DIR_NAME"))
        return QDir(ApplicationEnvironment().value("OWNET_APP_DATA_DIR_NAME"));
    else
    {
        QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));

        if (!dir.exists("OwNetClient"))
            dir.mkdir("OwNetClient");

        dir.cd("OwNetClient");
        return dir;
    }
}
