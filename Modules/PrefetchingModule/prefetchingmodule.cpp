#include "prefetchingmodule.h"
#include "prefetchingservice.h"
#include "iproxyconnection.h"

#include "prefetchingjob.h"

void PrefetchingModule::init(IProxyConnection *proxyConnection)
{

    proxyConnection->registerService( new PrefetchingService(proxyConnection,this) );
    proxyConnection->registerJob(new PrefetchingJob(proxyConnection, this));
}
