#ifndef FIREWALLDISCOVERYJOB_H
#define FIREWALLDISCOVERYJOB_H

#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;

class FirewallDiscoveryJob : public IJobAction
{
    Q_OBJECT
public:
    explicit FirewallDiscoveryJob();

    int interval() { return 10 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection) { m_proxyConnection = proxyConnection; }

private:
    QMutex m_pingMutex;
    IProxyConnection *m_proxyConnection;
};

#endif // FIREWALLDISCOVERYJOB_H
