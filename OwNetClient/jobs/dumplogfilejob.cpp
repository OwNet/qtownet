#include "dumplogfilejob.h"

#include "messagehelper.h"

DumpLogFileJob::DumpLogFileJob(QObject *parent)
    : Job(500, parent)
{
}

void DumpLogFileJob::execute()
{
    if (!m_activeMutex.tryLock())
        return;

    MessageHelper::writeLogFileToDisk();

    m_activeMutex.unlock();
}
