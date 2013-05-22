#include "proxytrafficcounterjob.h"

#include "proxytrafficcounter.h"
#include "webdownloadsmanager.h"

ProxyTrafficCounterJob::ProxyTrafficCounterJob(QObject *parent)
    : Job(5 * 1000, parent)
{
}

void ProxyTrafficCounterJob::execute()
{
    WebDownloadsManager::instance()->trafficCounter()->takeCurrentTrafficSnapshot();
}
