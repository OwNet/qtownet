#include "sessionmodule.h"

#include "service.h"

void SessionModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new Service(proxyConnection, this));
}

QList<IRestService *> *SessionModule::restServices()
{
    return new QList<IRestService *>(m_services);
}

Q_EXPORT_PLUGIN2(ownet_sessionmodule, SessionModule)
