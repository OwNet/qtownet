#ifndef CLEANSYNCJOURNALJOB_H
#define CLEANSYNCJOURNALJOB_H

#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;

class CleanSyncJournalJob : public IJobAction
{
    Q_OBJECT
public:
    enum {
        MaxNumberOfCacheItems = 5000
    };
    explicit CleanSyncJournalJob();

    int interval() { return 30 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection) { m_proxyConnection = proxyConnection; }

private:
    QMutex m_cleanMutex;
    IProxyConnection *m_proxyConnection;
};

#endif // CLEANSYNCJOURNALJOB_H
