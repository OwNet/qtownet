#include "initializer.h"
#include "messagehelper.h"
#include "settingsinitializer.h"
#include "applicationdatastorage.h"

#include <QCoreApplication>

Initializer::Initializer()
{
}

void Initializer::init()
{
    QCoreApplication::setOrganizationName("The Reconnected");
    QCoreApplication::setApplicationName("OwNet Client");

    SettingsInitializer().init();

    m_databaseInitializer.init();
    m_proxyInitializer.init();
    m_moduleInitializer.init();
    m_jobInitializer.init();

    createPidFile();

    MessageHelper::debug("Proxy initialized and waiting for requests.");
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
