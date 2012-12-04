#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class ClientService : public QObject, public IRestService
{
    Q_OBJECT
public:
    explicit ClientService(IProxyConnection *proxyConnection, QObject *parent = 0);

    QString name() const { return "clients"; }

    QVariant *processRequest(IBus *, IRequest *request);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // CLIENTSERVICE_H
