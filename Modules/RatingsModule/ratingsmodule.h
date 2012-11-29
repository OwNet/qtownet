#ifndef RATINGSMODULE_H
#define RATINGSMODULE_H

#include <QObject>
#include <QList>

#include "imodule.h"

class RatingsModule : public QObject, public IModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.thereconnected.OwNet.IModule/1.0")
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *proxyConnection);
    QList<IRestService*> *restServices();

private:
    QList<IRestService*> m_services;
};

#endif // RECOMMENDATIONSMODULE_H
