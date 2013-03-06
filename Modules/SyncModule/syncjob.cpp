#include "syncjob.h"

#include "syncclient.h"

SyncJob::SyncJob(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
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
