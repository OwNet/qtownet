#ifndef IMODULE_H
#define IMODULE_H

#include <QtPlugin>

class IRestService;
class IService;
class IJobAction;
class IProxyConnection;

/**
 * @brief The IModule class Interface for Modules (Abstract class)
 */
class IModule
{
public:
    virtual void init(IProxyConnection *) = 0;
};

Q_DECLARE_INTERFACE(IModule,
                    "com.thereconnected.OwNet.IModule/1.0")

#endif // IMODULE_H
