#ifndef SYNCMODULE_H
#define SYNCMODULE_H

#include <QObject>

#include "imodule.h"

class SyncModule : public QObject, public IModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.thereconnected.OwNet.IModule/1.0")
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *proxyConnection);
    QList<IJobAction *> *jobs();

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SYNCMODULE_H
