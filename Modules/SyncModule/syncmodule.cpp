#include "syncmodule.h"

#include "syncjob.h"
#include "syncserver.h"

void SyncModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;
    SyncServer::instance()->setProxyConnection(m_proxyConnection);
}

QList<IJobAction *> *SyncModule::jobs()
{
    QList<IJobAction *> *jobs = new QList<IJobAction *>;
    jobs->append(new SyncJob(m_proxyConnection, this));
    return jobs;
}

