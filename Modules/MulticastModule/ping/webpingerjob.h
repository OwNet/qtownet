#ifndef WEBPINGERJOB_H
#define WEBPINGERJOB_H

#include <QObject>

#include "ijobaction.h"

class IProxyConnection;
class WebPinger;

class WebPingerJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    explicit WebPingerJob(IProxyConnection *proxyConnection, QObject *parent = 0);

    int interval() { return 15 * 1000; }
    void execute();

private:
    IProxyConnection *m_proxyConnection;
    WebPinger *m_pinger;
};

#endif // WEBPINGERJOB_H
