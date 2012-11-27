#include "databasesettings.h"

#include <QMap>
#include <QSqlQuery>
#include <QSqlRecord>

#include "databaseupdate.h"

QMap<QString, QString> *DatabaseSettings::m_cachedSettings = new QMap<QString, QString>();

DatabaseSettings::DatabaseSettings(QObject *parent) :
    QObject(parent)
{
}

void DatabaseSettings::setValue(const QString &key, const QString &value)
{
    DatabaseSettings::m_cachedSettings->insert(key, value);

    DatabaseUpdate update(false);
    IDatabaseUpdateQuery *query = update.createUpdateQuery("settings", IDatabaseUpdateQuery::Detect);
    query->setWhere("key", key);
    query->setColumnValue("key", key);
    query->setColumnValue("value", value);
    update.execute();
}

QString DatabaseSettings::value(const QString &key, const QString &defaultValue) const
{
    if (DatabaseSettings::m_cachedSettings->contains(key))
        return DatabaseSettings::m_cachedSettings->value(key);

    QSqlQuery query;
    query.prepare("SELECT value FROM settings WHERE key = :key");
    query.bindValue("key", key);

    if (query.exec() && query.first()) {
        QString value = query.value(query.record().indexOf("value")).toString();
        DatabaseSettings::m_cachedSettings->insert(key, value);
        return value;
    }

    return defaultValue;
}
