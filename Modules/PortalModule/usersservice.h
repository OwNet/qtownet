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
    IResponse *show(IRequest *req, const QString &textId);
    IResponse *create(IRequest *req);
    IResponse *edit(IRequest *req, const QString &textId);
    IResponse *del(IRequest *req, const QString &textId);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // USERSERVICE_H
