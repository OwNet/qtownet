#ifndef CACHELOCATIONS_H
#define CACHELOCATIONS_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QStringList>

class WebDownload;
class QIODevice;
class ProxyRequest;
class WebReader;
class ProxyHandlerSession;

class CacheLocations : public QObject
{
    Q_OBJECT
public:
    enum LocationType {
        LocalCache,
        Web,
        NetworkCache,
        Unknown
    };

    explicit CacheLocations(QObject *parent = 0);

    void addLocation(const QString &clientId, const QString &dateTime);
    void addLocation(const QString &clientId, const QDateTime &dateTime);
    void addLocalLocation();
    void removeLocation(const QString &clientId);
    void removeLocalLocation();
    bool containsLocation(const QString &clientId) const;
    bool isCacheAvailable() const;

    QPair<LocationType, QString> getLocationType(bool refresh);

    QStringList sortedLocations() const;

    void setCacheId(uint cacheId) { m_cacheId = cacheId; }

private:
    LocationType getLocationType(const QString &clientId, bool checkIfExists = true) const;

    uint m_cacheId;
    QMap<QString, QStringList > m_locations;
};

#endif // CACHELOCATIONS_H
