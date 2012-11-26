#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class Service : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit Service(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "users"; }

    QVariant *create(IBus *bus, IRequest *req);
    QVariant *show(IBus *bus, IRequest *req);
    QVariant *index(IBus *bus, IRequest *);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SERVICE_H
