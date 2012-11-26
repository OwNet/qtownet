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
    virtual ISession *session(QObject *parent = 0) = 0;
    virtual IDatabaseUpdate *databaseUpdate(QObject *parent = 0) = 0;
    virtual QSettings *settings(QObject *parent = 0) = 0;
    virtual IRequest *createRequest(IRequest::RequestType requestType, const QString &module, const QString &action = QString(), int id = -1, QObject *parent = 0) = 0;
};

#endif // IPROXYCONNECTION_H
