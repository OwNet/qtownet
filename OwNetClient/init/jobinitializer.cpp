 #include "jobinitializer.h"

#include "cleancachejob.h"
#include "proxytrafficcounterjob.h"

JobInitializer::JobInitializer()
    : m_cleanCacheJob(NULL), m_trafficCounterJob(NULL)
{
}

JobInitializer::~JobInitializer()
{
    if (m_cleanCacheJob)
        delete m_cleanCacheJob;

    if (m_trafficCounterJob)
        delete m_trafficCounterJob;
}

void JobInitializer::init()
{
    m_cleanCacheJob = new CleanCacheJob();

    m_trafficCounterJob = new ProxyTrafficCounterJob();
}
