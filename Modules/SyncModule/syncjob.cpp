#include "syncjob.h"

#include "syncclient.h"

SyncJob::SyncJob() :
    m_proxyConnection(NULL)
{
}

void SyncJob::execute()
{
    if (!m_syncMutex.tryLock())
        return;

    SyncClient client(m_proxyConnection);
    client.sync();

    m_syncMutex.unlock();
}
