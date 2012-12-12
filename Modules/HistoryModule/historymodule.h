#ifndef HISTORYMODULE_H
#define HISTORYMODULE_H

#include <QObject>

#include "imodule.h"

class HistoryModule : public QObject, public IModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.thereconnected.OwNet.IModule/1.0")
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *proxyConnection);
    QList<IService*> *services() { return new QList<IService *>(m_services); }

private:
    QList<IService*> m_services;
};

#endif // HISTORYMODULE_H
