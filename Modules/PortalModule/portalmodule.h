#ifndef PORTALMODULE_H
#define PORTALMODULE_H

#include <QObject>

#include "imodule.h"

class PortalModule : public QObject, public IModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.thereconnected.OwNet.IModule/1.0")
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *);
};

#endif // PORTALMODULE_H
