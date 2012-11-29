#include "proxyconnection.h"

#include "settings.h"
#include "databasesettings.h"
#include "databaseupdate.h"
#include "databaseselectquery.h"
#include "session.h"
#include "artificialrequest.h"
#include "qjson/parser.h"

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

QVariant ProxyConnection::fromJson(const QByteArray &content) const
{
    QJson::Parser parser;
    return parser.parse(content);
}
