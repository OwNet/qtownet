#include "proxyconnection.h"

#include "settings.h"
#include "databasesettings.h"
#include "synceddatabaseupdatequery.h"
#include "databaseselectquery.h"
#include "session.h"
#include "artificialrequest.h"
#include "requestrouter.h"
#include "jsondocument.h"
#include "irestservice.h"
#include "iservice.h"
#include "ijobaction.h"
#include "modulejob.h"
#include "messagehelper.h"
#include "uniqueidhelper.h"
#include "cachehelper.h"
#include "jobinitializer.h"

#include "proxydownloads.h"
#include "proxytrafficcounter.h"

#include <QApplication>

ProxyConnection::ProxyConnection(QObject *parent) :
    QObject(parent)
{
}

ISession *ProxyConnection::session(QObject *parent)
{
    return new Session(parent);
}

IDatabaseUpdateQuery *ProxyConnection::databaseUpdateQuery(const QString &table, QObject *parent, bool sync)
{
    if (sync)
        return new SyncedDatabaseUpdateQuery(table, DatabaseUpdateQuery::InsertOrUpdate, parent);
    return new DatabaseUpdateQuery(table, DatabaseUpdateQuery::InsertOrUpdate, parent);
}

IDatabaseSelectQuery *ProxyConnection::databaseSelect(const QString &table, QObject *parent)
{
    return new DatabaseSelectQuery(table, parent);
}

ISettings *ProxyConnection::settings(QObject *parent)
{
    return new Settings(parent);
}

IDatabaseSettings *ProxyConnection::databaseSettings(QObject *parent)
{
    return new DatabaseSettings(parent);
}

void ProxyConnection::registerDatabaseUpdateListener(IDatabaseUpdateListener *listener)
{
    DatabaseUpdateQuery::registerListener(listener);
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

void ProxyConnection::registerUidRestService(IUidRestService *service)
{
    RequestRouter::addService(service);
}

void ProxyConnection::registerJob(IJobAction* job)
{
    QThread *thread = JobInitializer::startJob(new ModuleJob(job));
    if (thread)
        job->moveToThread(thread);
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

/**
 * @brief Output a message by the given message type
 * @param message Body of the message
 * @param title Title of the message
 * @param messageType Type of the message
 */
void ProxyConnection::message(const QVariant &message, const QString &title, MessageType messageType) const
{
    switch (messageType) {
    case Debug:
        MessageHelper::debug(message);
        break;

    case Error:
        MessageHelper::error(title, message);
        break;

    case InformationPopup:
        MessageHelper::information(title, message.toString());
        break;

    case WarningPopup:
        MessageHelper::warning(title, message.toString());
        break;

    case CriticalPopup:
        MessageHelper::error(title, message.toString());
        break;
    }
}

int ProxyConnection::lastConnectionTraffic() const
{
    return ProxyDownloads::instance()->trafficCounter()->lastTraffic();
}

QString ProxyConnection::generateUniqueId() const
{
    return UniqueIdHelper::generate();
}

uint ProxyConnection::cacheId(const QString &url) const
{
    return CacheHelper::cacheId(url);
}

void ProxyConnection::quit() const
{
    qApp->quit();
}
