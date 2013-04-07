#ifndef HEARTBEATJOB_H
#define HEARTBEATJOB_H

#include "ijobaction.h"

class MulticastProtocol;
class IProxyConnection;
class QHostAddress;

class MulticastJob : public IJobAction
{
    Q_OBJECT

public:
    MulticastJob(QHostAddress *groupAddress, int port);

    int interval() { return 5 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection);

private:
    QHostAddress *m_groupAddress;
    int m_port;
    MulticastProtocol *m_protocol;
    IProxyConnection *m_proxyConnection;
};

#endif // HEARTBEATJOB_H
