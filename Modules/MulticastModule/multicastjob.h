#ifndef HEARTBEATJOB_H
#define HEARTBEATJOB_H

#include "ijobaction.h"

#include <QObject>

class QHostAddress;

class MulticastJob : public QObject, public IJobAction
{
    Q_OBJECT

public:
    MulticastJob(QHostAddress *groupAddress, int port, QObject *parent = 0);

    int interval() { return 5 * 1000; };
    void execute();

private:
    QHostAddress *m_groupAddress;
    int m_port;
};

#endif // HEARTBEATJOB_H
