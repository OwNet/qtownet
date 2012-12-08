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

    QVariant *index(IBus *bus, IRequest *);
    QVariant *show(IBus *bus, IRequest *req, int id);
    QVariant *create(IBus *bus, IRequest *req);
    QVariant *edit(IBus *, IRequest *, int id);
    QVariant *del(IBus *bus, IRequest *req, int id);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // USERSERVICE_H
