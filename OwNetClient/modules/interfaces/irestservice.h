#ifndef IRESTMODULE_H
#define IRESTMODULE_H

#include <QString>
#include "iservice.h"
#include "iresponse.h"
#include "irequest.h"


class IRestService : public IService
{
public:

    virtual IResponse *index(IRequest *req) { return req->response(IResponse::METHOD_NOT_ALLOWED); }

    // create element
    virtual IResponse *create(IRequest *req) { return req->response(IResponse::METHOD_NOT_ALLOWED); }

   //show element
    virtual IResponse *show(IRequest *req, const QString &id) { return req->response(IResponse::METHOD_NOT_ALLOWED); }

    // delete element
    virtual IResponse *del(IRequest *req, const QString &id) { return req->response(IResponse::METHOD_NOT_ALLOWED); }

    // edit element
    virtual IResponse *edit(IRequest *req, const QString &id) { return req->response(IResponse::METHOD_NOT_ALLOWED); }


};

#endif // IRESTMODULE_H
