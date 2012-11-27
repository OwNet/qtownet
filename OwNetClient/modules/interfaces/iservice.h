#ifndef ISERVICES_H
#define ISERVICES_H

#include <QString>

class IService
{
public:
    virtual QString name() const = 0;

    virtual QByteArray *processRequest(IBus *bus, IRequest *req) = 0;
};

#endif // ISERVICES_H
