#ifndef SESSIONMODULE_H
#define SESSIONMODULE_H

#include <QObject>
#include <QVariantMap>

#include "imodule.h"

class SessionModule : public QObject, public IModule
{
    Q_OBJECT
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *proxyConnection);
    QList<IRestService*> *restServices();

private:
    QList<IRestService *> m_services;
};

#endif // SESSIONMODULE_H
