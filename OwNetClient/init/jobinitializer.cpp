 #include "jobinitializer.h"

#include "cleancachejob.h"

JobInitializer::JobInitializer()
{
    m_cleanCacheJob = NULL;
//    m_prefetchJob = NULL;
}

JobInitializer::~JobInitializer()
{
    if (m_cleanCacheJob)
        delete m_cleanCacheJob;
//    if (m_prefetchJob)
//        delete m_prefetchJob;
}

void JobInitializer::init()
{
    m_cleanCacheJob = new CleanCacheJob();
//   m_prefetchJob = new PrefetchJob();
}
