#include "centralservicesyncjob.h"

#include "centralservicereporter.h"
#include "centralserviceupdater.h"

CentralServiceSyncJob::CentralServiceSyncJob() :
    m_proxyConnection(NULL)
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
