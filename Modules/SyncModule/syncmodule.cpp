#include "syncmodule.h"

#include "iproxyconnection.h"
#include "syncjob.h"
#include "syncserver.h"

void SyncModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;
    proxyConnection->registerJob( new SyncJob(proxyConnection,this) );
}
