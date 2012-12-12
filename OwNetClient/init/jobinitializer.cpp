 #include "jobinitializer.h"

#include "cleancachejob.h"
#include "proxytrafficcounterjob.h"
#include "heartbeatjob.h"
#include "updaterecordssavejob.h"

JobInitializer::JobInitializer()
    : m_cleanCacheJob(NULL), m_trafficCounterJob(NULL), m_updateRecordsSaveJob(NULL)
{
}

JobInitializer::~JobInitializer()
{
    if (m_cleanCacheJob)
        delete m_cleanCacheJob;

    if (m_trafficCounterJob)
        delete m_trafficCounterJob;

    if (m_updateRecordsSaveJob)
        delete m_updateRecordsSaveJob;
}

void JobInitializer::init()
{
    m_cleanCacheJob = new CleanCacheJob();

    m_trafficCounterJob = new ProxyTrafficCounterJob();

    m_updateRecordsSaveJob = new UpdateRecordsSaveJob();
}
