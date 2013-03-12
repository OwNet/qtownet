#include "proxymodule.h"

#include "iproxyconnection.h"
#include "refreshservice.h"
#include "webpingerjob.h"

void ProxyModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;

    m_proxyConnection->registerService(new RefreshService(m_proxyConnection, this));

    m_proxyConnection->registerJob(new WebPingerJob(m_proxyConnection, this));
}