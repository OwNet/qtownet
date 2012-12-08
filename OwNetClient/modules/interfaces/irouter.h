#ifndef IROUTER_H
#define IROUTER_H

#include "iroute.h"

#define ROUTE(func, rest...) [&](IBus *bus, IRequest *req, IRoute::Match params) -> QVariant* { return func(bus,req,##rest); }
#define INT(x) params.captured(#x).toInt()
#define STR(x) params.captured(#x)
#define ROUTE_FN [&](IBus *bus, IRequest *req, IRoute::Match params) -> QVariant*



class IRouter
{
public:
    IRoute* addRoute(QString url);
};

#endif // IROUTER_H
