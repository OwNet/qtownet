#include "settingsinitializer.h"

#include "settings.h"
#include "applicationenvironment.h"
#include "messagehelper.h"

#include <QDesktopServices>
#include <QApplication>
#include <QDir>

SettingsInitializer::SettingsInitializer(QObject *parent) :
    QObject(parent)
{
}

void SettingsInitializer::init()
{
    if (ApplicationEnvironment().contains("OWNET_INI_DIR"))
    {
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, ApplicationEnvironment().value("OWNET_INI_DIR"));
        QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, ApplicationEnvironment().value("OWNET_INI_DIR"));
    }

    QDir dir;
    Settings settings;

    if (!settings.contains("application/data_folder_path")) {
        dir.setPath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
        settings.setValue("application/data_folder_path", dir.absoluteFilePath("OwNetClient"));
    }
    MessageHelper::debug(QObject::tr("Using data directory %1")
                         .arg(settings.value("application/data_folder_path").toString()));

    if (!settings.contains("application/resources_folder_path")) {
        dir.setPath(QApplication::applicationDirPath());
        settings.setValue("application/resources_folder_path", dir.absoluteFilePath("resources"));
    }
    MessageHelper::debug(QObject::tr("Using resources directory %1")
                         .arg(settings.value("application/resources_folder_path").toString()));
}