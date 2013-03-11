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

    QList<ProxyCacheLocation*> sortedLocations() const;
    
private:
    QMap<QString, QList<ProxyCacheLocation*>* > m_locations;
    QList<ProxyCacheLocation *> m_sortedLocations;
};

class ProxyCacheLocation : public QObject
{
    Q_OBJECT
public:
    explicit ProxyCacheLocation(const QString &clientId, QObject *parent = 0);

    bool isLocal() const;
    bool isWeb() const;
    QString clientId() const { return m_clientId; }

private:
    QString m_clientId;
};

#endif // PROXYCACHELOCATION_H
