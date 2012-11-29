#include "stubconnection.h"

StubConnection::StubConnection()
{
}

ISession *StubConnection::session(QObject *parent)
{
    return NULL;
}

IDatabaseUpdate *StubConnection::databaseUpdate(QObject *parent)
{
    return NULL;
}

IDatabaseSettings *StubConnection::databaseSettings(QObject *parent)
{
    return NULL;
}

IDatabaseSelectQuery *StubConnection::databaseSelect(const QString &table, QObject *parent)
{
    return NULL;
}

QSettings *StubConnection::settings(QObject *parent)
{
    return NULL;
}

IRequest *StubConnection::createRequest(IRequest::RequestType requestType, const QString &module, const QString &action, int id, QObject *parent)
{
    return NULL;
}

QVariant StubConnection::fromJson(const QByteArray &content) const
{
    return QVariant();
}
