#ifndef PINGJOB_H
#define PINGJOB_H

#include "ijobaction.h"

#include <QMutex>

class IProxyConnection;
class PingServer;

class PingJob : public IJobAction
{
    Q_OBJECT
public:
    explicit PingJob();

    int interval() { return 10 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection);

private:
    IProxyConnection *m_proxyConnection;
    PingServer *m_pingServer;
    QMutex m_pingMutex;
};

#endif // PINGJOB_H
