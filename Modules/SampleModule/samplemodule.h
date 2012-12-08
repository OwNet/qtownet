#ifndef SAMPLEMODULE_H
#define SAMPLEMODULE_H

#include <QObject>

#include "imodule.h"

class SampleModule : public QObject, public IModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.thereconnected.OwNet.IModule/1.0")
    Q_INTERFACES(IModule)

public:
    void init(IProxyConnection *);
};

#endif // SAMPLEMODULE_H
