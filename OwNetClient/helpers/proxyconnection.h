#ifndef PROXYCONNECTION_H
#define PROXYCONNECTION_H

#include <QObject>

#include "iproxyconnection.h"

class ProxyConnection : public QObject, public IProxyConnection
{
    Q_OBJECT
public:
    explicit ProxyConnection(QObject *parent = 0);

    ISession *session(QObject *parent = 0);
    IDatabaseUpdate *databaseUpdate(QObject *parent = 0);
    IDatabaseSelectQuery *databaseSelect(const QString &table, QObject *parent = 0);
    QSettings *settings(QObject *parent = 0);
    IDatabaseSettings *databaseSettings(QObject *parent = 0);
    IRequest *createRequest(IRequest::RequestType requestType, const QString &module, const QString &action = QString(), int id = -1, QObject *parent = 0);
    QVariant fromJson(const QByteArray &content, bool *ok = NULL) const;
    QByteArray toJson(const QVariant &content) const;
    QVariant *callModule(IRequest *req);

    void registerService(IService* service);
    void registerRestService(IRestService* service);
    void registerJob(IJobAction* job);
};

#endif // PROXYCONNECTION_H
