#include "samplemodule.h"
#include "iproxyconnection.h"

#include "sampleservice.h"

void SampleModule::init(IProxyConnection *proxyConnection)
{
    proxyConnection->registerRestService( new SampleService(proxyConnection,this) );
}


