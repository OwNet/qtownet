#ifndef CACHEEXCEPTIONSSERVICE_H
#define CACHEEXCEPTIONSSERVICE_H

#include <QObject>

#include "iservice.h"

class IProxyConnection;

class CacheExceptionsService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit CacheExceptionsService(IProxyConnection *proxyConnection, QObject *parent = 0);

    virtual QString name() const { return "cache_exceptions"; }
    void init(IRouter* router);

private:
    IResponse *add(IRequest *request);
    IResponse *remove(IRequest *request);
    IResponse *check(IRequest *request);

    IProxyConnection *m_proxyConnection;
};

#endif // CACHEEXCEPTIONSSERVICE_H
