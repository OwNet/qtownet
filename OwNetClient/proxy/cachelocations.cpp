#include "cachelocations.h"

#include "session.h"
#include "databasesettings.h"
#include "cachefolder.h"

#include <QDateTime>
#include <QStringList>

CacheLocations::CacheLocations(QObject *parent) :
    QObject(parent)
{
}

void CacheLocations::addLocation(const QString &clientId, const QString &dateTime)
{
    addLocation(clientId, QDateTime::fromString(dateTime, Qt::ISODate));
}

void CacheLocations::addLocation(const QString &clientId, const QDateTime &dateTime)
{
    QDateTime roundedDateTime;
    roundedDateTime.setDate(dateTime.date());
    QTime time = dateTime.time();
    roundedDateTime.setTime(QTime(time.hour(), 0));

    QString key = roundedDateTime.toString(Qt::ISODate);

    if (m_locations.contains(key)) {
        QStringList list = m_locations.value(key);
        bool inserted = false;

        switch (getLocationType(clientId, false)) {
        case LocalCache:
            list.prepend(clientId);
            break;

        case NetworkCache:
            for (int i = 0; i < list.count(); ++i) {
                if (getLocationType(list.at(i), false) == LocalCache)
                    continue;
                list.insert(i, clientId);
                inserted = true;
                break;
            }
            if (!inserted)
                list.append(clientId);
            break;

        default:
            list.append(clientId);
            break;
        }

        m_locations.insert(key, list);
    } else {
        m_locations.insert(key, QStringList(clientId));
    }
}

void CacheLocations::addLocalLocation()
{
    addLocation(DatabaseSettings().clientId(), QDateTime::currentDateTime());
}

void CacheLocations::removeLocation(const QString &clientId)
{
    foreach (QString key, m_locations.keys()) {
        QStringList list = m_locations.value(key);
        for (int i = list.count() - 1; i >= 0; --i) {
            if (list.at(i) == clientId)
                list.removeAt(i);
        }
        m_locations.insert(key, list);
    }
}

void CacheLocations::removeLocalLocation()
{
    removeLocation(DatabaseSettings().clientId());
}

bool CacheLocations::containsLocation(const QString &clientId) const
{
    return sortedLocations().contains(clientId);
}

bool CacheLocations::isCacheAvailable() const
{
    switch (getLocationType(false)) {
    case LocalCache:
    case NetworkCache:
        return true;
    }
    return false;
}

QPair<CacheLocations::LocationType, QString> CacheLocations::getLocationType(bool refresh)
{
    if (!refresh) {
        LocationType locationType = Unknown;
        foreach (QString clientId, sortedLocations()) {
            if ((locationType = getLocationType(clientId)) == Unknown)
                continue;

            return QPair<LocationType, QString>(locationType, clientId);
        }
    }

    return QPair<LocationType, QString>(Web, "WEB");
}

QStringList CacheLocations::sortedLocations() const
{
    QStringList results;
    QStringList keys(m_locations.keys());
    keys.sort();

    for (int i = keys.count() - 1; i >= 0; --i)
        results.append(m_locations.value(keys.at(i)));

    return results;
}

CacheLocations::LocationType CacheLocations::getLocationType(const QString &clientId, bool checkIfExists) const
{
    if (DatabaseSettings().clientId() == clientId) {
        if (!checkIfExists)
            return LocalCache;

        QObject parent;
        QFile *file = CacheFolder().cacheFile(m_cacheId, 0, &parent);
        if (file->exists())
            return LocalCache;

        return Unknown;
    } else if (clientId != "WEB") {
        if (!checkIfExists || Session().availableClients().contains(clientId))
            return NetworkCache;

        return Unknown;
    }
    if (!checkIfExists || Session().isOnline())
        return Web;
    return Unknown;
}
