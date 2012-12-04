#ifndef REQUESTROUTER_H
#define REQUESTROUTER_H

#include <QObject>

class IRestService;
class IService;
class IBus;
class IRequest;

class RequestRouter : public QObject
{
    Q_OBJECT
public:
    RequestRouter(const QString &serviceName, QObject *parent = 0);

    QByteArray *processRequest(IBus *bus, IRequest *req) const;
    QVariant *processRestRequest(IBus *bus, IRequest *req) const;

    static void addRoute(IService *service);
    static void addRoute(IRestService *service);
    
private:
    IService *m_service;
    IRestService *m_restService;

    static QMap<QString, IService*> *m_services;
    static QMap<QString, IRestService*> *m_restServices;
};

#endif // REQUESTROUTER_H
