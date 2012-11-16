#ifndef PROXYTRAFFICCOUNTERJOB_H
#define PROXYTRAFFICCOUNTERJOB_H

#include "job.h"

class ProxyTrafficCounterJob : public Job
{
public:
    ProxyTrafficCounterJob(QObject *parent = 0);

    void execute();
};

#endif // PROXYTRAFFICCOUNTERJOB_H
