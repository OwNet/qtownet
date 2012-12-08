#include "proxyconnection.h"

#include "settings.h"
#include "databasesettings.h"
#include "databaseupdate.h"
#include "databaseselectquery.h"
#include "session.h"
#include "artificialrequest.h"
#include "artificialbus.h"
#include "requestrouter.h"
#include "qjson/parser.h"
#include "qjson/serializer.h"

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

IRequest *ProxyConnection::createRequest(IRequest::RequestType requestType, const QString &module, const QString &action, int id, QObject *parent)
{
    return new ArtificialRequest(requestType, module, action, id, parent);
}

QVariant ProxyConnection::fromJson(const QByteArray &content, bool *ok) const
{
    QJson::Parser parser;
    return parser.parse(content, ok);
}

QByteArray ProxyConnection::toJson(const QVariant &content) const
{
    if (content.isNull())
        return QString("{}").toUtf8();

    QJson::Serializer serializer;
    return serializer.serialize(content);
}

/**
 * @brief ProxyConnection::callModule Function for modules,
 * they call it when they need to communicate with other modules
 * @param req
 * @return processedRequest from module in byte array
 */
QVariant *ProxyConnection::callModule(IRequest *req)
{
    ArtificialBus bus;
    RequestRouter router(req->module());
    return router.processRestRequest(&bus, req);
}
