#ifndef CENTRALSERVICESYNCJOB_H
#define CENTRALSERVICESYNCJOB_H

#include <QObject>
#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;

class CentralServiceSyncJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    explicit CentralServiceSyncJob(IProxyConnection *proxyConnection, QObject *parent = 0);

    int interval() { return 15 * 1000; }

    void execute();

private:
    QMutex m_syncMutex;
    IProxyConnection *m_proxyConnection;
};

#endif // CENTRALSERVICESYNCJOB_H
