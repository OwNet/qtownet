#ifndef IRESTMODULE_H
#define IRESTMODULE_H

#include <QString>

class QVariant;
class IRequest;
class IBus;

class IRestService
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

#endif // IRESTMODULE_H
