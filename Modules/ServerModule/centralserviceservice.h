#ifndef CENTRALSERVICESERVICE_H
#define CENTRALSERVICESERVICE_H

#include "iservice.h"

#include <QObject>

class IProxyConnection;

class CentralServiceService : public QObject, public IService
{
    Q_OBJECT
public:
    CentralServiceService(IProxyConnection *proxyConnection, QObject *parent = NULL);

    QString name() const { return "central_service"; }
    void init(IRouter *router);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // CENTRALSERVICESERVICE_H
