#ifndef HEARTBEATJOB_H
#define HEARTBEATJOB_H

#include "ijobaction.h"

#include <QObject>

class QHostAddress;
class MulticastProtocol;

class MulticastJob : public QObject, public IJobAction
{
    Q_OBJECT

public:
    MulticastJob(QHostAddress *groupAddress, int port, MulticastProtocol *protocol,
                 QObject *parent = 0);

    int interval() { return 5 * 1000; };
    void execute();

private:
    QHostAddress *m_groupAddress;
    int m_port;
    MulticastProtocol *m_protocol;
};

#endif // HEARTBEATJOB_H
