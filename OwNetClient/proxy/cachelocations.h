#ifndef CACHELOCATIONS_H
#define CACHELOCATIONS_H

#include <QObject>
#include <QMap>

class ProxyWebDownload;
class QIODevice;
class ProxyRequest;
class ProxyWebReader;
class ProxyHandlerSession;

class CacheLocations : public QObject
{
    Q_OBJECT
public:
    explicit CacheLocations(QObject *parent = 0);

    void addLocation(const QString &clientId, const QString &dateTime, ProxyWebDownload *location);
    void addLocation(const QString &clientId, const QDateTime &dateTime, ProxyWebDownload *location);
    void removeLocation(const QString &clientId);
    bool containsLocation(const QString &clientId) const;

    QIODevice *getStream(ProxyRequest *request, ProxyWebReader *reader, ProxyHandlerSession *handlerSession, bool *finished, bool refresh);

    QList<ProxyWebDownload*> sortedLocations() const;

private slots:
    void currentDownloadFinished();
    void currentDownloadFailed();

private:
    ProxyWebDownload *m_local;
    ProxyWebDownload *m_currentDownload;
    QMap<QString, QList<ProxyWebDownload*>* > m_locations;
    QList<ProxyWebDownload *> m_sortedLocations;
};

#endif // CACHELOCATIONS_H
