#include "jobinitializer.h"

#include "cleancachejob.h"

JobInitializer::JobInitializer()
{
    m_cleanCacheJob = NULL;
}

JobInitializer::~JobInitializer()
{
    if (m_cleanCacheJob)
        delete m_cleanCacheJob;
}

void JobInitializer::init()
{
    m_cleanCacheJob = new CleanCacheJob();
}
