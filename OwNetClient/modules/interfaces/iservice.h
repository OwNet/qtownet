#ifndef ISERVICES_H
#define ISERVICES_H

#include <QString>

class IBus;
class IRequest;
class QVariant;
class IRouter;

class IService
{
public:
    virtual QString name() const = 0;

    virtual void init(IRouter* router){}

    virtual QVariant *processRequest(IBus *bus, IRequest *req){ return NULL; }
};

#endif // ISERVICES_H
