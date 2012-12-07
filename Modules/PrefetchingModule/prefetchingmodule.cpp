#include "prefetchingmodule.h"

#include "service.h"
#include "iproxyconnection.h"

#include "prefetchjob.h"

void PrefetchingModule::init(IProxyConnection *proxyConnection)
{
    proxyConnection->registerService( new Service(proxyConnection,this) );

    m_prefetchJob = new PrefetchJob();
    proxyConnection->registerJob(m_prefetchJob);
}
