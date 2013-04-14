#include "webpingerjob.h"

#include "webpinger.h"

WebPingerJob::WebPingerJob() :
    m_proxyConnection(NULL),
    m_pinger(NULL)
{
}

void WebPingerJob::execute()
{
    if (!m_pinger)
        m_pinger = new WebPinger(m_proxyConnection, this);

    m_pinger->ping();
}
