#ifndef IRESTMODULE_H
#define IRESTMODULE_H

#include <QString>
#include <QtPlugin>

class QByteArray;
class IRequest;
class IBus;

class IRestModule
{
public:
    virtual QString name() const = 0;

    virtual QByteArray *index(IBus *, IRequest *) { return NULL; }

    // create element
    virtual QByteArray *create(IBus *, IRequest *) { return NULL; }

    // show element
    virtual QByteArray *show(IBus *, IRequest *) { return NULL; }

    // delete element
    virtual QByteArray *del(IBus *, IRequest *) { return NULL; }

    // edit element
    virtual QByteArray *edit(IBus *, IRequest *) { return NULL; }

    // other action
    virtual QByteArray *processRequest(IBus *, IRequest *) { return NULL; }
};

Q_DECLARE_INTERFACE(IRestModule,
                    "com.thereconnected.OwNet.IRestModule/1.0")

#endif // IRESTMODULE_H
