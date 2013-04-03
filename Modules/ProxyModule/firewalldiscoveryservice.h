#ifndef FIREWALLDISCOVERYSERVICE_H
#define FIREWALLDISCOVERYSERVICE_H

#include <QObject>

#include "iservice.h"

class IProxyConnection;

class FirewallDiscoveryService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit FirewallDiscoveryService(IProxyConnection *proxyConnection, QObject *parent = 0);

    virtual QString name() const { return "firewall"; }
    void init(IRouter* router);

private:
    IResponse *pingMe(IRequest *request);
    IResponse *ping(IRequest *request);

    IProxyConnection *m_proxyConnection;
};

#endif // FIREWALLDISCOVERYSERVICE_H
