#include "session.h"

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

void Session::clear()
{
    m_sessionData->clear();
}

bool Session::isLoggedIn() const
{
    return m_sessionData->contains("logged");
}
