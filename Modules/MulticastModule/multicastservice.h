#ifndef MULTICASTSERVICE_H
#define MULTICASTSERVICE_H

#include <QObject>

#include "iservice.h"
#include "multicastprotocol.h"

class IProxyConnection;

class MulticastService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit MulticastService(IProxyConnection *proxyConnection, MulticastProtocol *protocol,
                              QObject *parent = 0);

    virtual QString name() const { return "sync"; }
    void init(IRouter* router);

    IResponse *initialized(IRequest *request);

private:
    IProxyConnection *m_proxyConnection;
    MulticastProtocol *m_protocol;
};

#endif // MULTICASTSERVICE_H
