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
    QByteArray *create(IBus *bus, IRequest *req);

    // show element
    QByteArray *show(IBus *bus, IRequest *req);

    QByteArray *index(IBus *bus,  IRequest *);
    
};

#endif // USERMODULE_H
