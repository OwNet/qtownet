#ifndef IRESTMODULE_H
#define IRESTMODULE_H

#include <QString>
#include "iservice.h"
#include "iresponse.h"
#include "irequest.h"


class IRestService : public IService
{
public:

    virtual IResponse *index(IRequest *) { return NULL; }

    // create element
    virtual IResponse *create(IRequest *) { return NULL; }

    // show element
    virtual IResponse *show(IRequest *, uint id) { return NULL; }

    // delete element
    virtual IResponse *del(IRequest *, uint id) { return NULL; }

    // edit element
    virtual IResponse *edit(IRequest *, uint id) { return NULL; }

};

#endif // IRESTMODULE_H
