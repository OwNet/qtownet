#ifndef HEARTBEATJOB_H
#define HEARTBEATJOB_H

#include "ijobaction.h"

#include <QObject>

class QHostAddress;
class MulticastProtocol;
class IProxyConnection;

class MulticastJob : public QObject, public IJobAction
{
    Q_OBJECT

public:
    MulticastJob(QHostAddress *groupAddress, int port, MulticastProtocol *protocol, IProxyConnection *proxyConnection,
                 QObject *parent = 0);

    int interval() { return 5 * 1000; }
    void execute();

private:
    QHostAddress *m_groupAddress;
    int m_port;
    MulticastProtocol *m_protocol;
    IProxyConnection *m_proxyConnection;
};

#endif // HEARTBEATJOB_H
