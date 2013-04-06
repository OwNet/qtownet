#ifndef FIREWALLDISCOVERYJOB_H
#define FIREWALLDISCOVERYJOB_H

#include <QObject>
#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;

class FirewallDiscoveryJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    explicit FirewallDiscoveryJob(IProxyConnection *proxyConnection, QObject *parent = 0);

    int interval() { return 10 * 1000; }
    void execute();

private:
    QMutex m_pingMutex;
    IProxyConnection *m_proxyConnection;
};

#endif // FIREWALLDISCOVERYJOB_H
