#include "initializer.h"
#include "applicationproxyfactory.h"
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

    // manage proxies to stub network
    QNetworkProxyFactory::setApplicationProxyFactory(new ApplicationProxyFactory());

    SettingsInitializer().init();
    m_databaseInitializer.init();
    m_proxyInitializer.init();
    m_moduleInitializer.init();
    m_communicationInitializer.init();
    m_jobInitializer.init();

    MessageHelper::debug("Proxy initialized and waiting for requests.");

    // store current pid in pidfile
    QFile file(ApplicationDataStorage().appDataDirectory().absoluteFilePath("ownet.pid"));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << QString::number(QCoreApplication::applicationPid());
    file.close();
}
