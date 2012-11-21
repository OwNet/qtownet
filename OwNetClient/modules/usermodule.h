#ifndef USERMODULE_H
#define USERMODULE_H

#include <QObject>
#include <imodule.h>
#include <ibus.h>
#include <QByteArray>

class UserModule : public IModule
{
public:
    explicit UserModule();

    QString url() const { return "users"; }

private:
    // create element
    QByteArray* create(IBus *bus, IRequest *req);

    // show element
    QByteArray* show( IBus *bus, IRequest *req);

    //delete element
    QByteArray* del( IBus *bus, IRequest *req);

    //edit element
    QByteArray* edit( IBus *bus, IRequest *req);

    QByteArray* index( IBus *bus,  IRequest *req);
    
};

#endif // USERMODULE_H
