#ifndef APPSERVICE_H
#define APPSERVICE_H

#include "iservice.h"

#include <QObject>

class IProxyConnection;

class AppService : public QObject, public IService
{
    Q_OBJECT
public:
    explicit AppService(IProxyConnection *proxyConnection, QObject *parent = 0);

    virtual QString name() const { return "app"; }
    void init(IRouter* router);

private:
    IResponse *quit(IRequest *request);

    IProxyConnection *m_proxyConnection;
};

#endif // APPSERVICE_H
