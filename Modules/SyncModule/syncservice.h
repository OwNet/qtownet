#ifndef SYNCSERVICE_H
#define SYNCSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class SyncService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit SyncService(IProxyConnection *proxyConnection, QObject *parent = 0);

    virtual QString name() const { return "sync"; }
    QVariant *processRequest(IBus *bus, IRequest *request);

    QVariant *getUpdates(IBus *, IRequest *request);
    QVariant *availableRecords(IBus *, IRequest *);
    QVariant *uploadChanges(IBus *, IRequest *request);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SYNCSERVICE_H
