#include "proxyconnection.h"

#include "settings.h"
#include "databaseupdate.h"
#include "session.h"
#include "artificialrequest.h"

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

QSettings *ProxyConnection::settings(QObject *parent)
{
    return new Settings(parent);
}

IRequest *ProxyConnection::createRequest(IRequest::RequestType requestType, const QString &module, const QString &action, int id, QObject *parent)
{
    return new ArtificialRequest(requestType, module, action, id, parent);
}
