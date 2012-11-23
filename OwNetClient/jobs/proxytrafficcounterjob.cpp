#include "proxytrafficcounterjob.h"

#include "proxydownloads.h"
#include "proxytrafficcounter.h"

ProxyTrafficCounterJob::ProxyTrafficCounterJob(QObject *parent)
    : Job(5 * 1000, parent)
{
}

void ProxyTrafficCounterJob::execute()
{
    ProxyDownloads::instance()->trafficCounter()->takeCurrentTrafficSnapshot();
}
