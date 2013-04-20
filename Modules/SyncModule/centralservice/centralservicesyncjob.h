#ifndef CENTRALSERVICESYNCJOB_H
#define CENTRALSERVICESYNCJOB_H

#include <QObject>
#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;

class CentralServiceSyncJob : public IJobAction
{
    Q_OBJECT
public:
    explicit CentralServiceSyncJob();

    int interval() { return 15 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection) { m_proxyConnection = proxyConnection; }

private:
    QMutex m_syncMutex;
    IProxyConnection *m_proxyConnection;
};

#endif // CENTRALSERVICESYNCJOB_H
