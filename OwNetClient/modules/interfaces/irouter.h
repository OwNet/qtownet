#ifndef IROUTER_H
#define IROUTER_H

#include "iroute.h"

#define ROUTE(func, rest...) [&](IRequest *req, IRoute::Match params) -> IResponse* { return func(req,##rest); }
#define INT(x) params.captured(#x).toInt()
#define STR(x) params.captured(#x)
#define ROUTE_FN [&](IRequest *req, IRoute::Match params) -> IResponse*
#define DEFAULT_ROUTE_FN [&](IRequest *req) -> IResponse*

class IRouter
{
public:
    typedef std::function<IResponse* (IRequest *req)> DefaultCallback;

    virtual IRoute* addRoute(QString url) = 0;
    virtual void setDefaultRoute(DefaultCallback) = 0;
};

#endif // IROUTER_H
