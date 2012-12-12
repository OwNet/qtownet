#ifndef SYNCSERVICE_H
#define SYNCSERVICE_H

#include <QObject>

#include "iservice.h"

class IProxyConnection;

class SyncService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit SyncService(IProxyConnection *proxyConnection, QObject *parent = 0);

    virtual QString name() const { return "sync"; }
    void init(IRouter* router);

    IResponse *getUpdates(IRequest *request);
    IResponse *availableRecords(IRequest *);
    IResponse *uploadChanges(IRequest *request);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SYNCSERVICE_H
