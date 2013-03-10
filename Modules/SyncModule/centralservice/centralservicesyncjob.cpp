#include "centralservicesyncjob.h"

#include "centralservicereporter.h"
#include "centralserviceupdater.h"

CentralServiceSyncJob::CentralServiceSyncJob(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

/**
 * @brief Executed after the timer ticks. Syncs with the central service.
 */
void CentralServiceSyncJob::execute()
{
    if (!m_syncMutex.tryLock())
        return;

    /// Report
    CentralServiceReporter reporter(m_proxyConnection);
    reporter.reportSyncJournal();
    reporter.reportBrowsingHistory();

    /// Update
    CentralServiceUpdater updater(m_proxyConnection);
    updater.updateJournal();

    m_syncMutex.unlock();
}
