#include "proxyconnection.h"

#include "settings.h"
#include "databasesettings.h"
#include "databaseupdate.h"
#include "databaseselectquery.h"
#include "session.h"
#include "artificialrequest.h"
#include "requestrouter.h"
#include "jsondocument.h"
#include "irestservice.h"
#include "iservice.h"
#include "ijobaction.h"
#include "modulejob.h"

ProxyConnection::ProxyConnection(QObject *parent) :
    QObject(parent)
{
}

ISession *ProxyConnection::session(QObject *parent)
{
    return new Session(parent);
}

IDatabaseUpdate *ProxyConnection::databaseUpdate(QObject *parent)
{
    return new DatabaseUpdate(parent);
}

IDatabaseSelectQuery *ProxyConnection::databaseSelect(const QString &table, QObject *parent)
{
    return new DatabaseSelectQuery(table, parent);
}

QSettings *ProxyConnection::settings(QObject *parent)
{
    return new Settings(parent);
}

IDatabaseSettings *ProxyConnection::databaseSettings(QObject *parent)
{
    return new DatabaseSettings(parent);
}

IRequest *ProxyConnection::createRequest(IRequest::RequestType requestType, const QString &service, const QString &url, QObject *parent)
{
    return new ArtificialRequest(requestType, service, url, parent);
}

IRequest *ProxyConnection::createRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject *parent)
{
    return new ArtificialRequest(requestType, service, id, parent);
}

QVariant ProxyConnection::fromJson(const QByteArray &content, bool *ok) const
{
    return JsonDocument::fromJson(content,ok).toVariant();
}

QByteArray ProxyConnection::toJson(const QVariant &content) const
{
    return JsonDocument::fromVariant(content).toJson();
}

void ProxyConnection::registerService(IService* service)
{
    RequestRouter::addService(service);
}

void ProxyConnection::registerRestService(IRestService* service)
{
    RequestRouter::addService(service);
}

void ProxyConnection::registerJob(IJobAction* job)
{
    new ModuleJob(job, this);
}

/**
 * @brief ProxyConnection::callModule Function for modules,
 * they call it when they need to communicate with other modules
 * @param req
 * @return processedRequest from module in byte array
 */
IResponse *ProxyConnection::callModule(IRequest *req)
{
    return RequestRouter::processRequest(req);
}
