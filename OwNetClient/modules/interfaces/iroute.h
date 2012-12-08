#ifndef IROUTE_H
#define IROUTE_H

#include<QString>
#include<QMap>
#include<QRegularExpressionMatch>

#include "irequest.h"

class IBus;

class IRoute
{
public:
    typedef QRegularExpressionMatch Match;
    typedef std::function<QVariant* (IBus *bus, IRequest *req, Match params)> Callback;
    typedef QMap<IRequest::RequestType, Callback> CallbackMap;

    virtual IRoute* on(IRequest::RequestType method, Callback callback) = 0;
};

#endif // IROUTE_H
