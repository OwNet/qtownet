#include "recommendationsmodule.h"

#include "iproxyconnection.h"
#include "recommendationsservice.h"

void RecommendationsModule::init(IProxyConnection *proxyConnection)
{
    proxyConnection->registerRestService( new RecommendationsService(proxyConnection,this) );
}
