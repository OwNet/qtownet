#include "session.h"

#include "databasesettings.h"

#include <QMutex>

QVariantMap *Session::m_sessionData = new QVariantMap;
QMutex *Session::m_mutex = new QMutex;

Session::Session(QObject *parent)
    : QObject(parent)
{
}

void Session::setValue(const QString &key, const QVariant &value)
{
    m_mutex->lock();
    m_sessionData->insert(key, value);
    m_mutex->unlock();
}

QVariant Session::value(const QString &key, const QVariant &defaultValue) const
{
    m_mutex->lock();
    QVariant value = m_sessionData->value(key, defaultValue);
    m_mutex->unlock();
    return value;
}

bool Session::contains(const QString &key) const
{
    m_mutex->lock();
    bool contains = m_sessionData->contains(key);
    m_mutex->unlock();
    return contains;
}

void Session::clear()
{
    m_mutex->lock();
    m_sessionData->clear();
    m_mutex->unlock();
}

bool Session::isLoggedIn() const
{
    return contains("logged");
}

uint Session::userId() const
{
    return value("logged").toUInt();
}

QVariantMap Session::availableClients() const
{
    if (contains("available_clients"))
        return value("available_clients").toMap();
    return QVariantMap();
}

QString Session::serverId() const
{
    if (contains("server_id") && !value("server_id").isNull())
        return value("server_id").toString();
    return DatabaseSettings().clientId();
}

bool Session::isServer() const
{
    return serverId() == DatabaseSettings().clientId();
}

bool Session::isOnline() const
{
    return value("is_online", true).toBool();
}

bool Session::isRefreshSession() const
{
    return value("refresh_session", false).toBool();
}
