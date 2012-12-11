#ifndef ISERVICES_H
#define ISERVICES_H

#include <QString>
#include "irequest.h"
#include "iresponse.h"

class IRouter;

class IService
{
public:
    virtual QString name() const = 0;
    virtual void init(IRouter* router){}
};

#endif // ISERVICES_H
