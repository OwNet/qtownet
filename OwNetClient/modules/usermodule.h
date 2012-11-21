#ifndef USERMODULE_H
#define USERMODULE_H

#include "irestmodule.h"

class UserModule : public IRestModule
{
public:
    explicit UserModule();

    QString name() const { return "users"; }

private:
    // create element
    QVariant *create(IBus *bus, IRequest *req);

    // show element
    QVariant *show(IBus *bus, IRequest *req);

    QVariant *index(IBus *bus,  IRequest *);
    
};

#endif // USERMODULE_H
