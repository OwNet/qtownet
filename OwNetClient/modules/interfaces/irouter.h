#ifndef IROUTER_H
#define IROUTER_H

#include "iroute.h"

#define ROUTE(func,...) [&](IRequest *req, IRoute::Match params) -> IResponse* { return func(req,##__VA_ARGS__); }
#define INT(x) params.captured(#x).toInt()
#define UINT(x) params.captured(#x).toUInt()
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
