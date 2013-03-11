#include "proxycachelocations.h"

#include "databasesettings.h"

#include <QTime>
#include <QtCore/qmath.h>
#include <QStringList>

ProxyCacheLocations::ProxyCacheLocations(QObject *parent) :
    QObject(parent)
{
}

void ProxyCacheLocations::addLocation(const QString &clientId, const QString &dateTime)
{
    addLocation(clientId, QDateTime::fromString(dateTime, Qt::ISODate));
}

void ProxyCacheLocations::addLocation(const QString &clientId, const QDateTime &dateTime)
{
    QDateTime roundedDateTime;
    roundedDateTime.setDate(dateTime.date());
    QTime time = dateTime.time();
    roundedDateTime.setTime(QTime(time.hour(), 0));

    ProxyCacheLocation *location = new ProxyCacheLocation(clientId, this);
    QString key = roundedDateTime.toString(Qt::ISODate);

    if (m_locations.contains(key)) {
        QList<ProxyCacheLocation*> *list = m_locations.value(key);
        if (location->isLocalCache()) {
            list->prepend(location);
        } else if (location->isWeb()) {
            list->append(location);
        } else {
            bool inserted = false;
            for (int i = 0; i < list->count(); ++i) {
                if (list->at(i)->isLocalCache())
                    continue;
                list->insert(i, location);
                inserted = true;
                break;
            }
            if (!inserted)
                list->append(location);
        }
        m_locations.value(key)->append(location);
    } else {
        QList<ProxyCacheLocation*> *list = new QList<ProxyCacheLocation*>();
        list->append(location);
        m_locations.insert(key, list);
    }
}

void ProxyCacheLocations::removeLocation(const QString &clientId)
{
    foreach (QString key, m_locations.keys()) {
        QList<ProxyCacheLocation *> *list = m_locations.value(key);
        for (int i = list->count() - 1; i >= 0; --i) {
            if (list->at(i)->clientId() == clientId)
                list->removeAt(i);
        }
    }
}

QList<ProxyCacheLocation *> ProxyCacheLocations::sortedLocations() const
{
    QList<ProxyCacheLocation *> results;
    QStringList keys(m_locations.keys());
    keys.sort();

    for (int i = keys.count() - 1; i >= 0; --i) {
        results.append(*(m_locations.value(keys.at(i))));
    }

    return results;
}

ProxyCacheLocation::ProxyCacheLocation(const QString &clientId, QObject *parent) :
    QObject(parent),
    m_clientId(clientId),
    m_locationType(NetworkCache)
{
    if (m_clientId == "WEB")
        m_locationType = Web;
    else if (m_clientId == DatabaseSettings().clientId())
        m_locationType = LocalCache;
}
