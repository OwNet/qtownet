#ifndef PINGSERVICE_H
#define PINGSERVICE_H

#include <QObject>

#include "iservice.h"

class PingServer;

class PingService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit PingService(PingServer *pingServer, QObject *parent = 0);

    virtual QString name() const { return "ping"; }
    void init(IRouter* router);

private:
    IResponse *availableClients(IRequest *request);
    IResponse *hi(IRequest *request);

    PingServer *m_pingServer;
};

#endif // PINGSERVICE_H
