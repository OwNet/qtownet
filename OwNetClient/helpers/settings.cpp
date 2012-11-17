#include "settings.h"

QMap<QString, QVariant> *Settings::m_cachedSettings = new QMap<QString, QVariant>();

Settings::Settings(QObject *parent)
    : QSettings(parent)
{
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    Settings::m_cachedSettings->insert(key, value);
    QSettings::setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    if (Settings::m_cachedSettings->contains(key))
        return Settings::m_cachedSettings->value(key);
    if (QSettings::contains(key)) {
        QVariant value = QSettings::value(key, defaultValue);
        Settings::m_cachedSettings->insert(key, value);
        return value;
    }
    return defaultValue;
}
