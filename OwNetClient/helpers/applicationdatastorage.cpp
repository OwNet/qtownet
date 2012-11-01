#include "applicationdatastorage.h"

#include <QDesktopServices>

ApplicationDataStorage::ApplicationDataStorage(QObject *parent) :
    QObject(parent)
{
}

QDir ApplicationDataStorage::appDataDirectory()
{
    QDir dir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    if (!dir.exists("OwNetClient"))
        dir.mkdir("OwNetClient");

    dir.cd("OwNetClient");
    return dir;
}
