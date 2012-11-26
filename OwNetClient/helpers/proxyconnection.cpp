#include "proxyconnection.h"

#include "settings.h"
#include "databaseupdate.h"
#include "session.h"

ProxyConnection::ProxyConnection(QObject *parent) :
    QObject(parent)
{
}

ISession *ProxyConnection::session()
{
    return new Session(this);
}

IDatabaseUpdate *ProxyConnection::databaseUpdate()
{
    return new DatabaseUpdate(this);
}

QSettings *ProxyConnection::settings()
{
    return new Settings(this);
}
