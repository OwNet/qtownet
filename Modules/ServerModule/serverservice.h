#ifndef SERVERSERVICE_H
#define SERVERSERVICE_H

#include <QObject>

#include "iservice.h"

class IProxyConnection;

class ServerService : public QObject, public IService
{
    Q_OBJECT
public:
    ServerService(IProxyConnection *proxyConnection, QObject *parent = NULL);

    QString name() const { return "server"; }
    void init(IRouter *router);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SERVERSERVICE_H
