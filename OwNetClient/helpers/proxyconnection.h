#ifndef PROXYCONNECTION_H
#define PROXYCONNECTION_H

#include <QObject>

#include "iproxyconnection.h"

class ProxyConnection : public QObject, public IProxyConnection
{
    Q_OBJECT
public:
    explicit ProxyConnection(QObject *parent = 0);

    /// Module initialization
    void registerService(IService* service);
    void registerRestService(IRestService* service);
    void registerJob(IJobAction* job);

    /// App storage
    ISession *session(QObject *parent = 0);
    IDatabaseUpdateQuery *databaseUpdateQuery(const QString &table, QObject *parent = 0, bool sync = true);
    IDatabaseSelectQuery *databaseSelect(const QString &table, QObject *parent = 0);
    QSettings *settings(QObject *parent = 0);
    IDatabaseSettings *databaseSettings(QObject *parent = 0);

    /// Proxy requests
    IRequest *createRequest(IRequest::RequestType requestType, const QString &service, const QString &url = QString(), QObject *parent = 0);
    IRequest *createRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject *parent = 0);
    IResponse *callModule(IRequest *req);

    /// Helpers
    QVariant fromJson(const QByteArray &content, bool *ok = NULL) const;
    QByteArray toJson(const QVariant &content) const;
    void debugMessage(const QVariant &message) const;
    int lastConnectionTraffic() const;
    QString generateUniqueId() const;
    uint cacheId(const QString &url) const;
};

#endif // PROXYCONNECTION_H
