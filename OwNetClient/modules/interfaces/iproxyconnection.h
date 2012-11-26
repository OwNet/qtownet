#ifndef IPROXYCONNECTION_H
#define IPROXYCONNECTION_H

#include <QString>

#include "irequest.h"

class ISession;
class IDatabaseUpdate;
class QSettings;

class IProxyConnection
{
public:
    virtual ISession *session() = 0;
    virtual IDatabaseUpdate *databaseUpdate() = 0;
    virtual QSettings *settings() = 0;
    virtual IRequest *createRequest(IRequest::RequestType requestType, const QString &module, const QString &action = QString(), int id = -1) = 0;
};

#endif // IPROXYCONNECTION_H
