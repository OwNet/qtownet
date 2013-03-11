#include "session.h"

#include "databasesettings.h"

QVariantMap *Session::m_sessionData = new QVariantMap;

Session::Session(QObject *parent)
    : QObject(parent)
{
}

void Session::setValue(const QString &key, const QVariant &value)
{
    m_sessionData->insert(key, value);
}

QVariant Session::value(const QString &key) const
{
    return m_sessionData->value(key);
}

bool Session::contains(const QString &key) const
{
    return m_sessionData->contains(key);
}

void Session::clear()
{
    m_sessionData->clear();
}

bool Session::isLoggedIn() const
{
    return m_sessionData->contains("logged");
}

uint Session::userId() const
{
    return m_sessionData->value("logged").toUInt();
}

QVariantMap Session::availableClients() const
{
    if (m_sessionData->contains("available_clients"))
        return m_sessionData->value("available_clients").toMap();
    return QVariantMap();
}

QString Session::serverId() const
{
    if (m_sessionData->contains("server_id") && !m_sessionData->value("server_id").isNull())
        return m_sessionData->value("server_id").toString();
    return DatabaseSettings().clientId();
}

bool Session::isServer() const
{
    return serverId() == DatabaseSettings().clientId();
}

bool Session::isOnline() const
{
    return m_sessionData->value("is_online", true).toBool();
}

bool Session::isRefreshSession() const
{
    return m_sessionData->value("refresh_session", false).toBool();
}
