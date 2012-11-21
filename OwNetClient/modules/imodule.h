#ifndef IMODULE_H
#define IMODULE_H

#include <QString>
#include <QtPlugin>

class IBus;
class IRequest;

/**
 * @brief The IModule class Interface for Modules (Abstract class)
 */
class IModule
{
public:
    virtual QString name() const = 0;

    virtual QByteArray *processRequest(IBus *bus, IRequest *req) = 0;
};

Q_DECLARE_INTERFACE(IModule,
                    "com.thereconnected.OwNet.IModule/1.0")

#endif // IMODULE_H
