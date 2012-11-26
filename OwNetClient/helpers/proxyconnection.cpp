#include "proxyconnection.h"

#include "settings.h"
#include "databaseupdate.h"
#include "session.h"
#include "artificialrequest.h"

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

IRequest *ProxyConnection::createRequest(IRequest::RequestType requestType, const QString &module, const QString &action, int id)
{
    return new ArtificialRequest(requestType, module, action, id, this);
}
