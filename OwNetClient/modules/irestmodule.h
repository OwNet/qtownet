#ifndef IRESTMODULE_H
#define IRESTMODULE_H

#include <QString>
#include <QtPlugin>

class QVariant;
class IRequest;
class IBus;

class IRestModule
{
public:
    virtual QString name() const = 0;

    virtual QVariant *index(IBus *, IRequest *) { return NULL; }

    // create element
    virtual QVariant *create(IBus *, IRequest *) { return NULL; }

    // show element
    virtual QVariant *show(IBus *, IRequest *) { return NULL; }

    // delete element
    virtual QVariant *del(IBus *, IRequest *) { return NULL; }

    // edit element
    virtual QVariant *edit(IBus *, IRequest *) { return NULL; }

    // other action
    virtual QVariant *processRequest(IBus *, IRequest *) { return NULL; }
};

Q_DECLARE_INTERFACE(IRestModule,
                    "com.thereconnected.OwNet.IRestModule/1.0")

#endif // IRESTMODULE_H
