#ifndef IPROXYCONNECTION_H
#define IPROXYCONNECTION_H

#include <QString>

#include "irequest.h"

class ISession;
class IDatabaseUpdateQuery;
class QSettings;
class IDatabaseSettings;
class IDatabaseSelectQuery;
class IService;
class IRestService;
class IJobAction;

class IProxyConnection
{
public:
    virtual ISession *session(QObject *parent = 0) = 0;
    virtual IDatabaseUpdateQuery *databaseUpdateQuery(const QString &table, QObject *parent = 0, bool sync = true) = 0;
    virtual IDatabaseSelectQuery *databaseSelect(const QString &table, QObject *parent = 0) = 0;
    virtual QSettings *settings(QObject *parent = 0) = 0;
    virtual IDatabaseSettings *databaseSettings(QObject *parent = 0) = 0;
    virtual IRequest *createRequest(IRequest::RequestType requestType, const QString &service, const QString &url = QString(), QObject *parent = 0) = 0;
    virtual IRequest *createRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject *parent = 0) = 0;
    virtual QVariant fromJson(const QByteArray &content, bool *ok = NULL) const = 0;
    virtual QByteArray toJson(const QVariant &content) const = 0;
    virtual void debugMessage(const QVariant &message) const = 0;
    virtual int lastConnectionTraffic() const = 0;

    virtual void registerService(IService*) = 0;
    virtual void registerRestService(IRestService*) = 0;
    virtual void registerJob(IJobAction*) = 0;

    /**
     * @brief CallModule to enable calls between modules
     * @param req
     * @return response byte array
     */
    virtual IResponse *callModule(IRequest *req) = 0;
};

#endif // IPROXYCONNECTION_H
