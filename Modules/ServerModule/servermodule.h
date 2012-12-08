#ifndef SERVERMODULE_H
#define SERVERMODULE_H

#include <QObject>

#include "imodule.h"

class ServerModule : public QObject, public IModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.thereconnected.OwNet.IModule/1.0")
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *proxyConnection);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SERVERMODULE_H
