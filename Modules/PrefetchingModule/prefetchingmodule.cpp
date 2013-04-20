#include "prefetchingmodule.h"
#include "prefetchingservice.h"
#include "downloadordersservice.h"
#include "iproxyconnection.h"

#include "prefetchingjob.h"

void PrefetchingModule::init(IProxyConnection *proxyConnection)
{
    proxyConnection->registerRestService(new DownloadOrdersService(proxyConnection, this));
    proxyConnection->registerService( new PrefetchingService(proxyConnection,this) );
    proxyConnection->registerJob(new PrefetchingJob);
}
