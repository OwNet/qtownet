#include "portalmodule.h"
#include "iproxyconnection.h"

#include "groupsservice.h"
#include "messagesservice.h"
#include "ratingsservice.h"
#include "sessionservice.h"
#include "usersservice.h"
#include "historyservice.h"
#include "recommendationsservice.h"
#include "activitiesservice.h"

void PortalModule::init(IProxyConnection *proxyConnection)
{            
    proxyConnection->registerRestService( new GroupsService(proxyConnection,this) );
    proxyConnection->registerUidRestService( new MessagesService(proxyConnection,this) );
    proxyConnection->registerUidRestService( new RatingsService(proxyConnection,this) );
    proxyConnection->registerUidRestService( new RecommendationsService(proxyConnection,this) );
    proxyConnection->registerRestService( new UsersService(proxyConnection,this) );
    proxyConnection->registerUidRestService( new SessionService(proxyConnection,this) );
    proxyConnection->registerUidRestService( new ActivitiesService(proxyConnection,this) );

    proxyConnection->registerService( new HistoryService(proxyConnection,this) );


}
