#include "settingsinitializer.h"

#include "settings.h"
#include "applicationenvironment.h"
#include "applicationproxyfactory.h"
#include "messagehelper.h"

#include <QStandardPaths>
#include <QDir>

SettingsInitializer::SettingsInitializer(QObject *parent) :
    QObject(parent)
{
}

void SettingsInitializer::init()
{
    if (ApplicationEnvironment().contains("OWNET_TEST_ENVIRONMENT"))
    {
        QSettings::setDefaultFormat(QSettings::IniFormat);
        QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, ApplicationEnvironment().value("OWNET_TEST_ENVIRONMENT"));
    }

    QDir dir;
    Settings settings;

    if (!settings.contains("application/data_folder_path")) {
        dir.setPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
        settings.setValue("application/data_folder_path", dir.absoluteFilePath("OwNetClient"));
    }
    MessageHelper::debug(QObject::tr("Using data directory %1")
                         .arg(settings.value("application/data_folder_path").toString()));

    if (!settings.contains("application/resources_folder_path")) {
        dir.setPath(QDir::currentPath());
        settings.setValue("application/resources_folder_path", dir.absoluteFilePath("resources"));
    }    
    MessageHelper::debug(QObject::tr("Using resources directory %1")
                         .arg(settings.value("application/resources_folder_path").toString()));

    // manage proxies to stub network
    QNetworkProxyFactory::setApplicationProxyFactory(new ApplicationProxyFactory());
}
