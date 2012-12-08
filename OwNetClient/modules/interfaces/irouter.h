#ifndef IROUTER_H
#define IROUTER_H

#include "iroute.h"

#define ROUTE(func, rest...) [&](IRequest *req, IRoute::Match params) -> IResponse* { return func(req,##rest); }
#define INT(x) params.captured(#x).toInt()
#define STR(x) params.captured(#x)
#define ROUTE_FN [&](IRequest *req, IRoute::Match params) -> IResponse*



class IRouter
{
public:
    IRoute* addRoute(QString url);
};

#endif // IROUTER_H
