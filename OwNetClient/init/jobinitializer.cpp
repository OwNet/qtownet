 #include "jobinitializer.h"

#include "cleancachejob.h"
#include "proxytrafficcounterjob.h"
#include "updaterecordssavejob.h"
#include "dumplogfilejob.h"

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

    if (m_dumpLogFileJob)
        delete m_dumpLogFileJob;
}

void JobInitializer::init()
{
    m_cleanCacheJob = new CleanCacheJob();

    m_trafficCounterJob = new ProxyTrafficCounterJob();

    m_updateRecordsSaveJob = new UpdateRecordsSaveJob();

    m_dumpLogFileJob = new DumpLogFileJob();
}
