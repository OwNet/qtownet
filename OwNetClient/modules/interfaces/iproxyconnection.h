#ifndef IPROXYCONNECTION_H
#define IPROXYCONNECTION_H

#include <QString>

#include "irequest.h"

class ISession;
class IDatabaseUpdateQuery;
class ISettings;
class IDatabaseSettings;
class IDatabaseSelectQuery;
class IService;
class IRestService;
class IJobAction;
class IDatabaseUpdateListener;
class IUidRestService;
class ICacheFolder;

class IProxyConnection
{
public:
    enum MessageType {
        Debug,
        Error,
        InformationPopup,
        WarningPopup,
        CriticalPopup
    };

    /// Module initialization
    virtual void registerService(IService*) = 0;
    virtual void registerRestService(IRestService*) = 0;
    virtual void registerUidRestService(IUidRestService*) = 0;
    virtual void registerJob(IJobAction*) = 0;
    virtual void registerDatabaseUpdateListener(IDatabaseUpdateListener *listener) = 0;

    /// App storage
    virtual ISession *session(QObject *parent = 0) = 0;
    virtual IDatabaseUpdateQuery *databaseUpdateQuery(const QString &table, QObject *parent = 0, bool sync = true) = 0;
    virtual IDatabaseSelectQuery *databaseSelect(const QString &table, QObject *parent = 0) = 0;
    virtual ISettings *settings(QObject *parent = 0) = 0;
    virtual IDatabaseSettings *databaseSettings(QObject *parent = 0) = 0;

    /// Proxy requests
    virtual IRequest *createRequest(IRequest::RequestType requestType, const QString &service, const QString &url = QString(), QObject *parent = 0) = 0;
    virtual IRequest *createRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject *parent = 0) = 0;

    /**
     * @brief CallModule to enable calls between modules
     * @param req
     * @return response byte array
     */
    virtual IResponse *callModule(IRequest *req) = 0;

    /// Helpers
    virtual QVariant fromJson(const QByteArray &content, bool *ok = NULL) const = 0;
    virtual QByteArray toJson(const QVariant &content) const = 0;
    virtual void message(const QVariant &message, const QString &title = QString(), MessageType messageType = Debug) const = 0;
    virtual QString generateUniqueId() const = 0;
    virtual int lastConnectionTraffic() const = 0;
    virtual uint cacheId(const QString &url) const = 0;
    virtual ICacheFolder *cacheFolder() const = 0;
    virtual void saveToCache(const QString &url, qint64 size, int numAccesses) const = 0;
    virtual bool isCacheAvailable(uint cacheId) const = 0;
    virtual void quit() const = 0;
};

#endif // IPROXYCONNECTION_H
