#ifndef SYNCJOB_H
#define SYNCJOB_H

#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;

class SyncJob : public IJobAction
{
    Q_OBJECT
public:
    explicit SyncJob();

    int interval() { return 15 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection) { m_proxyConnection = proxyConnection; }

private:
    QMutex m_syncMutex;
    IProxyConnection *m_proxyConnection;
};

#endif // SYNCJOB_H
