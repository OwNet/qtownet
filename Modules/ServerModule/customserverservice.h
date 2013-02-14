#ifndef CUSTOMSERVERSERVICE_H
#define CUSTOMSERVERSERVICE_H

#include <QObject>

#include "iservice.h"

class IProxyConnection;

class CustomServerService : public QObject, public IService
{
    Q_OBJECT
public:
    CustomServerService(IProxyConnection *proxyConnection, QObject *parent = NULL);

    QString name() const { return "custom_server"; }
    void init(IRouter *router);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // CUSTOMSERVERSERVICE_H
