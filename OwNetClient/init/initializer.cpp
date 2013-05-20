#include "initializer.h"

#include "messagehelper.h"
#include "applicationdatastorage.h"

#include "databaseinitializer.h"
#include "proxyinitializer.h"
#include "moduleinitializer.h"
#include "jobinitializer.h"
#include "settingsinitializer.h"
#include "workspaceinitializer.h"
#include "applicationenvironment.h"
#include "settings.h"

#include <QCoreApplication>
#include <QTextStream>

Initializer::Initializer(QObject *parent)
    : QObject(parent)
{
}

void Initializer::init()
{
    QCoreApplication::setOrganizationName("The Reconnected");
    QCoreApplication::setApplicationName("OwNet Client");

    ApplicationEnvironment().init();

    (new SettingsInitializer(this))->init();
    (new WorkspaceInitializer(this))->init();
    (new DatabaseInitializer(this))->init();
    (new ProxyInitializer(this))->init();
    (new ModuleInitializer(this))->init();
    (new JobInitializer(this))->init();

    createPidFile();

    MessageHelper::debug("READY");
}

/*
 * @brief Store current pid in pidfile.
 */
void Initializer::createPidFile()
{
    QFile file(ApplicationDataStorage().appDataDirectory().absoluteFilePath("ownet.pid"));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << QString::number(QCoreApplication::applicationPid());
    file.close();
}
