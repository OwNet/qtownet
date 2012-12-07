#include "synclock.h"

QMutex *SyncLock::m_syncMutex = new QMutex;

SyncLock::SyncLock()
    : QMutexLocker(m_syncMutex)
{
}
