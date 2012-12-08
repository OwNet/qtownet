#ifndef STUBCONNECTION_H
#define STUBCONNECTION_H

#include "../OwNetClient/modules/interfaces/iproxyconnection.h"

class StubConnection : public IProxyConnection
{
public:
    StubConnection();

    ISession *session(QObject *parent = 0);
    IDatabaseUpdate *databaseUpdate(QObject *parent = 0);
    IDatabaseSettings *databaseSettings(QObject *parent = 0);
    IDatabaseSelectQuery *databaseSelect(const QString &table, QObject *parent);
    QSettings *settings(QObject *parent = 0);
    IRequest *createRequest(IRequest::RequestType requestType, const QString &service, const QString &url = QString(), QObject *parent = 0);
    IRequest *createRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject *parent = 0);
    QVariant fromJson(const QByteArray &content, bool *ok = NULL) const;
    QByteArray toJson(const QVariant &content) const;
    QVariant *callModule(IRequest *req);

    void registerService(IService* service);
    void registerRestService(IRestService* service);
    void registerJob(IJobAction* job) {}

    IService* getService(QString name);
    IRestService* getRestService(QString name);

private:
    QMap<QString,IService*> m_services;
    QMap<QString,IRestService*> m_restServices;
};

#endif // STUBCONNECTION_H
