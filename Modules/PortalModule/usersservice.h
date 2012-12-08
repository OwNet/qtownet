#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;
class IResponse;

class UsersService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit UsersService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "users"; }

    IResponse *index(IRequest *req);
    IResponse *show(IRequest *req, int id);
    IResponse *create(IRequest *req);
//    IResponse *edit(IRequest *, int id);
//    IResponse *del(IRequest *req, int id);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // USERSERVICE_H
