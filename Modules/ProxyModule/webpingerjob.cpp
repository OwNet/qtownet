#include "webpingerjob.h"

#include "webpinger.h"

WebPingerJob::WebPingerJob(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_pinger = new WebPinger(proxyConnection, this);
}

void WebPingerJob::execute()
{
    m_pinger->ping();
}
