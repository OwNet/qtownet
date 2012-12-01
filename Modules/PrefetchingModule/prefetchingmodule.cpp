#include "prefetchingmodule.h"

#include "service.h"

#include "prefetchjob.h"

void PrefetchingModule::init(IProxyConnection *proxyConnection)
{
    m_services.append(new Service(proxyConnection, this));
    m_prefetchJob = new PrefetchJob();
    m_jobs.append(m_prefetchJob);
}

QList<IService *> *PrefetchingModule::services()
{
    return new QList<IService *>(m_services);
}

QList<IJobAction *> *PrefetchingModule::jobs()
{
    return new QList<IJobAction *>(m_jobs);
}

PrefetchJob* PrefetchingModule::prefetchJob()
{
    return m_prefetchJob;
}
