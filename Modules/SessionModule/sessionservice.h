#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class SessionService : public QObject, public IRestService
{
    Q_OBJECT
public:
    SessionService(IProxyConnection *proxyConnection, QObject *parent = NULL);

    QString name() const { return "session"; }
    QVariant *create(IBus *bus, IRequest *req);
    QVariant *del(IBus *bus, IRequest *);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SERVICE_H
