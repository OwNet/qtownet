#ifndef WEBPINGERJOB_H
#define WEBPINGERJOB_H

#include "ijobaction.h"

class IProxyConnection;
class WebPinger;

class WebPingerJob : public IJobAction
{
    Q_OBJECT
public:
    explicit WebPingerJob();

    int interval() { return 15 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection) { m_proxyConnection = proxyConnection; }

private:
    IProxyConnection *m_proxyConnection;
    WebPinger *m_pinger;
};

#endif // WEBPINGERJOB_H
