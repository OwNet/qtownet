#ifndef HEARTBEATJOB_H
#define HEARTBEATJOB_H

#include "job.h"

class QHostAddress;

class HeartbeatJob : public Job
{
public:
    HeartbeatJob(QHostAddress *groupAddress, int port, QObject *parent = 0);

    void execute();

private:
    QHostAddress *m_groupAddress;
    int m_port;
};

#endif // HEARTBEATJOB_H
