#ifndef SESSIONSERVICE_H
#define SESSIONSERVICE_H

#include <QObject>

#include "iuidrestservice.h"
#include "iresponse.h"

class IProxyConnection;

class SessionService : public QObject, public IUidRestService
{
    Q_OBJECT
public:
    SessionService(IProxyConnection *proxyConnection, QObject *parent = NULL);

    QString name() const { return "session"; }

    void init(IRouter *router);

    IResponse *create(IRequest *req);
    IResponse *logout(IRequest *req);
    IResponse *index(IRequest *req);


private:
    IProxyConnection *m_proxyConnection;

    bool checkUserPassword(QString password, uint user_id);
};

#endif // SESSIONSERVICE_H
