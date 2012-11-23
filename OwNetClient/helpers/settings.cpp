#include "settings.h"

QMap<QString, QVariant> *Settings::m_cachedSettings = new QMap<QString, QVariant>();

Settings::Settings(QObject *parent)
    : QSettings(parent)
{
}

/**
 * @brief Set value for key in the QSettings and add it to cached values.
 * @param key Key for the value
 * @param value Value of the setting
 */
void Settings::setValue(const QString &key, const QVariant &value)
{
    Settings::m_cachedSettings->insert(key, value);
    QSettings::setValue(key, value);
}

/**
 * @brief Get value for key from cached values if available or from the QSettings.
 * Adds the value to cached settings.
 * @param key Key for the value
 * @param defaultValue Default value if not available
 * @return
 */
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
