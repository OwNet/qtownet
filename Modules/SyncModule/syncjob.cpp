#include "syncjob.h"

#include <QDebug>

SyncJob::SyncJob(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    execute();
}

void SyncJob::execute()
{
    if (!m_syncMutex.tryLock())
        return;

    m_syncMutex.unlock();
}
