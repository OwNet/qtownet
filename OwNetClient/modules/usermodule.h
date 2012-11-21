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
    QByteArray *create(IBus *bus, IRequest *req);

    // show element
    QByteArray *show(IBus *bus, IRequest *req);

    QByteArray *index(IBus *bus,  IRequest *);
    
};

#endif // USERMODULE_H
