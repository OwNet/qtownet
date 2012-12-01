#include "sessionmodule.h"

#include "sessionservice.h"

void SessionModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new SessionService(proxyConnection, this));
}

QList<IRestService *> *SessionModule::restServices()
{
    return new QList<IRestService *>(m_services);
}
