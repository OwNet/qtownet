#include "syncjob.h"

#include "syncclient.h"

SyncJob::SyncJob(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    execute();
}

void SyncJob::execute()
{
    return;
    if (!m_syncMutex.tryLock())
        return;

    SyncClient(m_proxyConnection).update();

    m_syncMutex.unlock();
}
