#ifndef SYNCSERVICE_H
#define SYNCSERVICE_H

#include <QObject>

#include "iservice.h"

class IProxyConnection;
class RefreshSession;

class RefreshService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit RefreshService(IProxyConnection *proxyConnection, QObject *parent = 0);

    virtual QString name() const { return "refresh"; }
    void init(IRouter* router);

private:
    IResponse *start(IRequest *request);
    IResponse *stop(IRequest *request);

    IProxyConnection *m_proxyConnection;
    RefreshSession *m_refreshSession;
};

#endif // SYNCSERVICE_H
