#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include <QObject>

#include "iservice.h"

class IProxyConnection;

class ClientService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit ClientService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "clients"; }
    void init(IRouter *router);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // CLIENTSERVICE_H
