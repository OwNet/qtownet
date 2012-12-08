#ifndef SYNCLOCK_H
#define SYNCLOCK_H

#include <QMutexLocker>

class SyncLock : public QMutexLocker
{
public:
    SyncLock();

private:
    static QMutex *m_syncMutex;
};

#endif // SYNCLOCK_H
