#include "initializer.h"
#include "applicationproxyfactory.h"
#include "messagehelper.h"

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

    m_databaseInitializer.init();
    m_proxyInitializer.init();
    m_moduleInitializer.init();
    m_jobInitializer.init();

    MessageHelper::debug("Proxy initialized and waiting for requests.");
}
