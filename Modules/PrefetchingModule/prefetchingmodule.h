#ifndef PREFETCHINGMODULE_H
#define PREFETCHINMODULE_H

#include <QObject>

#include "imodule.h"

class PrefetchJob;

class PrefetchingModule : public QObject, public IModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.thereconnected.OwNet.IModule/1.0")
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *proxyConnection);    
    PrefetchJob* prefetchJob() { return m_prefetchJob; }

private:    
    PrefetchJob* m_prefetchJob;
};


#endif // PREFETCHINGMODULE_H
