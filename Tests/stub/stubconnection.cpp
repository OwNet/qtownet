#include "stubconnection.h"
#include "../OwNetClient/modules/interfaces/iservice.h"
#include "../OwNetClient/modules/interfaces/irestservice.h"

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

IRequest *StubConnection::createRequest(IRequest::RequestType requestType, const QString &service, const QString &url, QObject *parent)
{
    return NULL;
}

IRequest *StubConnection::createRequest(IRequest::RequestType requestType, const QString &service, const int id, QObject *parent)
{
    return NULL;
}


QVariant StubConnection::fromJson(const QByteArray &content, bool *ok) const
{
    return QVariant();
}

QByteArray StubConnection::toJson(const QVariant &content) const
{
    return QByteArray();
}

QVariant *StubConnection::callModule(IRequest *req)
{
    return NULL;
}

void StubConnection::registerService(IService *service)
{
    m_services.insert(service->name(),service);
}

void StubConnection::registerRestService(IRestService *service)
{
    m_services.insert(service->name(),service);
    m_restServices.insert(service->name(),service);
}

IService *StubConnection::getService(QString name)
{
    return m_services.value(name);
}


IRestService *StubConnection::getRestService(QString name)
{
    return m_restServices.value(name);
}
