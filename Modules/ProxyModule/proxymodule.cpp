#include "proxymodule.h"

#include "iproxyconnection.h"
#include "refreshservice.h"
#include "webpingerjob.h"
#include "cacheexceptionsservice.h"
#include "firewalldiscoveryjob.h"
#include "firewalldiscoveryservice.h"

void ProxyModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;

    m_proxyConnection->registerService(new RefreshService(m_proxyConnection, this));
    m_proxyConnection->registerService(new CacheExceptionsService(m_proxyConnection, this));
    m_proxyConnection->registerService(new FirewallDiscoveryService(m_proxyConnection, this));

    m_proxyConnection->registerJob(new WebPingerJob);
    m_proxyConnection->registerJob(new FirewallDiscoveryJob);
}
