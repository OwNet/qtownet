#ifndef IROUTER_H
#define IROUTER_H

#include "route.h"

#define ROUTE(func, rest...) [&](IBus *bus, IRequest *req, Route::Match params) -> QVariant* { return func(bus,req,##rest); }
#define INT(x) params.captured(#x).toInt()
#define STR(x) params.captured(#x)

class IRouter
{
public:
    Route* addRoute(QString url);
};

#endif // IROUTER_H
