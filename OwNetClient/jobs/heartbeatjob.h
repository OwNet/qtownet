#ifndef HEARTBEATJOB_H
#define HEARTBEATJOB_H

#include "job.h"

class HeartbeatJob : public Job
{
public:
    HeartbeatJob(QObject *parent = 0);

    void execute();
};

#endif // HEARTBEATJOB_H
