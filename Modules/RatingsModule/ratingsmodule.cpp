#include "ratingsmodule.h"

#include "ratingsservice.h"

void RatingsModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new RatingsService(proxyConnection, this));
}

QList<IRestService *> *RatingsModule::restServices()
{
    return new QList<IRestService *>(m_services);
}

Q_EXPORT_PLUGIN2(ownet_ratingsmodule, RatingsModule)
