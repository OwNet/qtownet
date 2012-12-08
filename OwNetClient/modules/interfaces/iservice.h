#ifndef ISERVICES_H
#define ISERVICES_H

#include <QString>

class IBus;
class IRequest;
class IResponse;
class IRouter;

class IService
{
public:
    virtual QString name() const = 0;

    virtual void init(IRouter* router){}

    virtual IResponse *processRequest(IBus *bus, IRequest *req){ return NULL; }
};

#endif // ISERVICES_H
