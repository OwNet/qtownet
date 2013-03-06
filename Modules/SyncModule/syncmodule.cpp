#include "syncmodule.h"

#include "iproxyconnection.h"
#include "syncjob.h"
#include "syncserver.h"
#include "syncservice.h"
#include "centralservicesyncjob.h"

void SyncModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;

    m_proxyConnection->registerService(new SyncService(m_proxyConnection, this));

    m_proxyConnection->registerJob(new SyncJob(m_proxyConnection, this));
    m_proxyConnection->registerJob(new CentralServiceSyncJob(m_proxyConnection, this));
}
