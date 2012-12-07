#ifndef SERVERSERVICE_H
#define SERVERSERVICE_H

#include <QObject>

#include "irestservice.h"

class IProxyConnection;

class ServerService : public QObject, public IRestService
{
    Q_OBJECT
public:
    ServerService(IProxyConnection *proxyConnection, QObject *parent = NULL);

    QString name() const { return "server"; }

    QVariant *processRequest(IBus *, IRequest *request);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SERVERSERVICE_H
