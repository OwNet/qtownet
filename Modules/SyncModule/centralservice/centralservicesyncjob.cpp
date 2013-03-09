#include "centralservicesyncjob.h"

#include "centralservicereporter.h"

CentralServiceSyncJob::CentralServiceSyncJob(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void CentralServiceSyncJob::execute()
{
    if (!m_syncMutex.tryLock())
        return;

    CentralServiceReporter reporter(m_proxyConnection);
    reporter.reportSyncJournal();
    reporter.reportBrowsingHistory();

    m_syncMutex.unlock();
}
