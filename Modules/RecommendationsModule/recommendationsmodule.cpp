#include "recommendationsmodule.h"

#include "recommendationsservice.h"

void RecommendationsModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new RecommendationsService(proxyConnection, this));
}

QList<IRestService *> *RecommendationsModule::restServices()
{
    return new QList<IRestService *>(m_services);
}

