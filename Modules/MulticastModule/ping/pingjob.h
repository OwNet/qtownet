#ifndef PINGJOB_H
#define PINGJOB_H

#include "ijobaction.h"

#include <QObject>
#include <QMutex>

class IProxyConnection;
class PingServer;

class PingJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    explicit PingJob(PingServer *server, IProxyConnection *proxyConnection, QObject *parent = 0);

    int interval() { return 10 * 1000; }
    void execute();

private:
    IProxyConnection *m_proxyConnection;
    PingServer *m_pingServer;
    QMutex m_pingMutex;
};

#endif // PINGJOB_H
