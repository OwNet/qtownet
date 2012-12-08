#ifndef IRESTMODULE_H
#define IRESTMODULE_H

#include <QString>
#include "iservice.h"

class QVariant;
class IRequest;
class IBus;

class IRestService : public IService
{
public:

    virtual QVariant *index(IBus *, IRequest *) { return NULL; }

    // create element
    virtual QVariant *create(IBus *, IRequest *) { return NULL; }

    // show element
    virtual QVariant *show(IBus *, IRequest *, int id) { return NULL; }

    // delete element
    virtual QVariant *del(IBus *, IRequest *, int id) { return NULL; }

    // edit element
    virtual QVariant *edit(IBus *, IRequest *, int id) { return NULL; }

};

#endif // IRESTMODULE_H
