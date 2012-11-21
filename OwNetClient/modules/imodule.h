#ifndef IMODULE_H
#define IMODULE_H

#include "ibus.h"

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QtPlugin>

/**
 * @brief The IModule class Interface for Modules (Abstract class)
 */
class IModule
{
public:
    virtual QString url() const = 0;

    QByteArray* processRequest(IBus *bus, IRequest *req);

private:
    virtual QByteArray *index(IBus *, IRequest *) { return new QByteArray(); }

    // create element
    virtual QByteArray *create(IBus *, IRequest *) { return new QByteArray(); }

    // show element
    virtual QByteArray *show(IBus *, IRequest *) { return new QByteArray(); }

    //delete element
    virtual QByteArray *del(IBus *, IRequest *) { return new QByteArray(); }

    //edit element
    virtual QByteArray *edit(IBus *, IRequest *) { return new QByteArray(); }
};

Q_DECLARE_INTERFACE(IModule,
                     "com.thereconnected.OwNet.IModule/1.0")

#endif // IMODULE_H
