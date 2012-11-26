#include "usersmodule.h"

#include "service.h"

void UsersModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new Service(proxyConnection, this));
}

QList<IRestService *> *UsersModule::restServices()
{
    return new QList<IRestService *>(m_services);
}

Q_EXPORT_PLUGIN2(ownet_usersmodule, UsersModule)
