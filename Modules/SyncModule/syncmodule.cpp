#include "syncmodule.h"

#include "syncjob.h"
#include "syncserver.h"
#include "syncservice.h"

void SyncModule::init(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;
}

QList<IJobAction *> *SyncModule::jobs()
{
    QList<IJobAction *> *jobs = new QList<IJobAction *>;
    jobs->append(new SyncJob(m_proxyConnection, this));
    return jobs;
}

QList<IRestService *> *SyncModule::restServices()
{
    QList<IRestService *> *services = new QList<IRestService *>;
    services->append(new SyncService(m_proxyConnection, this));
    return services;
}

