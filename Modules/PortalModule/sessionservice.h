#ifndef SESSIONSERVICE_H
#define SESSIONSERVICE_H

#include <QObject>

#include "irestservice.h"
#include "iresponse.h"

class IProxyConnection;

class SessionService : public QObject, public IRestService
{
    Q_OBJECT
public:
    SessionService(IProxyConnection *proxyConnection, QObject *parent = NULL);

    QString name() const { return "session"; }
    IResponse *create(IRequest *req);
    IResponse *del(IRequest *req, uint id);

private:
    IProxyConnection *m_proxyConnection;

    bool checkUserPassword(QString password, uint user_id);
};

#endif // SESSIONSERVICE_H
