#include "historymodule.h"

#include "historyservice.h"

void HistoryModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new HistoryService(proxyConnection, this));
}


