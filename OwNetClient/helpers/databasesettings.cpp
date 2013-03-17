#include "databasesettings.h"

#include <QMap>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QMutex>

#include "databaseupdatequery.h"
#include "databaseselectquery.h"

QMap<QString, QString> *DatabaseSettings::m_cachedSettings = new QMap<QString, QString>();
int DatabaseSettings::m_numClientIdsGenerated = 0;

DatabaseSettings::DatabaseSettings(QObject *parent) :
    QObject(parent)
{
    clientId();
}

void DatabaseSettings::setValue(const QString &key, const QString &value) const
{
    DatabaseSettings::m_cachedSettings->insert(key, value);

    DatabaseUpdateQuery query("settings", IDatabaseUpdateQuery::InsertOrUpdate);
    query.singleWhere("key", key);
    query.setColumnValue("key", key);
    query.setColumnValue("value", value);
    query.executeQuery();
}

QString DatabaseSettings::value(const QString &key, const QString &defaultValue) const
{
    if (DatabaseSettings::m_cachedSettings->contains(key))
        return DatabaseSettings::m_cachedSettings->value(key);

    DatabaseSelectQuery query("settings");
    query.select("value");
    query.singleWhere("key", key);

    if (query.first()) {
        QString value = query.value("value").toString();
        DatabaseSettings::m_cachedSettings->insert(key, value);
        return value;
    }

    return defaultValue;
}

bool DatabaseSettings::hasClientId() const
{
    QString clientName = value("client_id");
    return !clientName.isEmpty();
}

QString DatabaseSettings::clientId() const
{
    if (!hasClientId())
        createClientId();

    QString id = value("client_id");
    return id;
}

void DatabaseSettings::createClientId() const
{
    setValue("client_id",
                      QString("C%1").arg(qHash(QString("c_%1-%2")
                                            .arg(QDateTime::currentDateTime().toString(Qt::ISODate))
                                            .arg(m_numClientIdsGenerated++))));
}

int DatabaseSettings::nextClientSyncRecordNumber()
{
    static QMutex syncRecordNumberMutex;
    QMutexLocker locker(&syncRecordNumberMutex);

    int clientRecordNumber = value("client_sync_record_number", "0").toInt();
    setValue("client_sync_record_number", QString::number(clientRecordNumber + 1));

    return clientRecordNumber;
}

void DatabaseSettings::clearCache() const
{
    DatabaseSettings::m_cachedSettings->clear();
}
