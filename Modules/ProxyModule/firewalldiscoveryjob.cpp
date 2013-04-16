#include "firewalldiscoveryjob.h"

#include "firewalldiscoverymanager.h"
#include "isession.h"
#include "iproxyconnection.h"
#include "idatabasesettings.h"
#include "irequest.h"
#include "iresponse.h"

FirewallDiscoveryJob::FirewallDiscoveryJob() :
    m_proxyConnection(NULL)
{
}

void FirewallDiscoveryJob::execute()
{
    if (!m_pingMutex.tryLock())
        return;

    FirewallDiscoveryManager *manager = new FirewallDiscoveryManager(m_proxyConnection);
    manager->checkFirewallStatus();

    m_pingMutex.unlock();
}
