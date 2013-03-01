#include "settingsinitializer.h"

#include "applicationenvironment.h"
#include "settings.h"

#include <QDir>
#include <QDebug>
#include <QStandardPaths>
#include <QCoreApplication>

SettingsInitializer::SettingsInitializer()
{
}

void SettingsInitializer::init()
{
    QCoreApplication::setOrganizationName("The Reconnected");
    QCoreApplication::setApplicationName("OwNet Client");

    if (ApplicationEnvironment().contains("OWNET_TEST_ENVIRONMENT"))
    {
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope,
                ApplicationEnvironment().value("OWNET_TEST_ENVIRONMENT"));
    }

    QDir dir;
    Settings settings;

    if (!settings.contains("application/data_folder_path")) {
        dir.setPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
        settings.setValue("application/data_folder_path", dir.absoluteFilePath("OwNetClient"));
    }
    qDebug() << QObject::tr("Service: Data directory %1")
                         .arg(settings.value("application/data_folder_path").toString());

    if (!settings.contains("application/resources_folder_path")) {
        dir.setPath(QDir::currentPath());
        settings.setValue("application/resources_folder_path", dir.absoluteFilePath("resources"));
    }
    qDebug() << QObject::tr("Service: Resources directory %1")
                         .arg(settings.value("application/resources_folder_path").toString());
}
