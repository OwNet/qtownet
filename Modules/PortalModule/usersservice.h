#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class UsersService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit UsersService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "users"; }

    QVariant *create(IBus *bus, IRequest *req);
    QVariant *show(IBus *bus, IRequest *req);
    QVariant *index(IBus *bus, IRequest *);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // USERSERVICE_H
