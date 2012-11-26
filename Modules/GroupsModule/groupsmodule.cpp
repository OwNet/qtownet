#include "groupsmodule.h"

#include "groupsservice.h"

void GroupsModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new GroupsService(proxyConnection, this));
}

QList<IRestService *> *GroupsModule::restServices()
{
    return new QList<IRestService *>(m_services);
}

Q_EXPORT_PLUGIN2(ownet_groupsmodule, GroupsModule)
