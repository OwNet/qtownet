#ifndef SYNCJOB_H
#define SYNCJOB_H

#include <QObject>
#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;

class SyncJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    explicit SyncJob(IProxyConnection *proxyConnection, QObject *parent = 0);

    int interval() { return 60 * 1000; }

    void execute();

private:
    QMutex m_syncMutex;
    IProxyConnection *m_proxyConnection;
};

#endif // SYNCJOB_H
