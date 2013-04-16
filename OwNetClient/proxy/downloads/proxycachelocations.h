#ifndef PROXYCACHELOCATION_H
#define PROXYCACHELOCATION_H

#include <QObject>
#include <QDateTime>
#include <QMap>
#include <QList>

class ProxyCacheLocation;

class ProxyCacheLocations : public QObject
{
    Q_OBJECT
public:
    explicit ProxyCacheLocations(QObject *parent = 0);

    void addLocation(const QString &clientId, const QString &dateTime);
    void addLocation(const QString &clientId, const QDateTime &dateTime);
    void removeLocation(const QString &clientId);
    bool containsLocation(const QString &clientId) const;

    QList<ProxyCacheLocation*> sortedLocations() const;
    
private:
    QMap<QString, QList<ProxyCacheLocation*>* > m_locations;
    QList<ProxyCacheLocation *> m_sortedLocations;
};

class ProxyCacheLocation : public QObject
{
    Q_OBJECT
public:
    enum LocationTypes {
        LocalCache,
        NetworkCache,
        Web
    };
    explicit ProxyCacheLocation(const QString &clientId, QObject *parent = 0);

    LocationTypes locationType() const { return m_locationType; }
    bool isLocalCache() const { return m_locationType == LocalCache; }
    bool isWeb() const { return m_locationType == Web; }
    bool isNetworkCache() const { return m_locationType == NetworkCache; }

    QString clientId() const { return m_clientId; }

private:
    QString m_clientId;
    LocationTypes m_locationType;
};

#endif // PROXYCACHELOCATION_H
