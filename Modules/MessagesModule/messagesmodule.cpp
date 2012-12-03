#include "messagesmodule.h"

#include "messagesservice.h"

void MessagesModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new MessagesService(proxyConnection, this));
}

QList<IRestService *> *MessagesModule::restServices()
{
    return new QList<IRestService *>(m_services);
}

