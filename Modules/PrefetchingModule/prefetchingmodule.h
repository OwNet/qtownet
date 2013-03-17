#ifndef PREFETCHINGMODULE_H
#define PREFETCHINMODULE_H

#include <QObject>

#include "imodule.h"

class PrefetchingJob;

class PrefetchingModule : public QObject, public IModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.thereconnected.OwNet.IModule/1.0")
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *proxyConnection);    
    PrefetchingJob* prefetchJob() { return m_prefetchJob; }

private:    
    PrefetchingJob* m_prefetchJob;
};


#endif // PREFETCHINGMODULE_H
