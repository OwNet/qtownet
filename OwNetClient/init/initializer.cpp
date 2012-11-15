#include "initializer.h"
#include "applicationproxyfactory.h"

Initializer::Initializer()
{
}

void Initializer::init()
{
    QNetworkProxyFactory::setApplicationProxyFactory(new ApplicationProxyFactory());

    m_databaseInitializer.init();
    m_proxyInitializer.init();
    m_moduleInitializer.init();
    m_jobInitializer.init();
}
