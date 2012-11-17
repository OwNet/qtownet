#include "settingsinitializer.h"

#include "settings.h"

#include <QDesktopServices>
#include <QApplication>
#include <QDir>

SettingsInitializer::SettingsInitializer(QObject *parent) :
    QObject(parent)
{
}

void SettingsInitializer::init()
{
    QDir dir;
    Settings settings;

    if (!settings.contains("application/data_folder_path")) {
        dir.setPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
        settings.setValue("application/data_folder_path", dir.absoluteFilePath("OwNetClient"));
    }

    if (!settings.contains("application/resources_folder_path")) {
        dir.setPath(QApplication::applicationDirPath());
        settings.setValue("application/resources_folder_path", dir.absoluteFilePath("resources"));
    }
}
