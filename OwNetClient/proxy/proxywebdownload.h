#ifndef PROXYWEBDOWNLOAD_H
#define PROXYWEBDOWNLOAD_H

#include "cachelocations.h"

#include <QObject>
#include <QMutex>
#include <QAbstractSocket>
#include <QMap>

class QIODevice;
class ProxyRequest;
class ProxyHandlerSession;
class ProxyWebReader;
class QFile;
class WebSocket;

class ProxyWebDownload : public QObject
{
    Q_OBJECT
public:
    enum Type {
        CacheFile,
        WebStream,
        NetworkStream,
        Unknown
    };

    ProxyWebDownload(uint cacheId, QObject *parent = 0);
    
    QIODevice *getStream(ProxyRequest *request, ProxyWebReader *reader, ProxyHandlerSession *session, bool refresh, bool *finished);

    bool exists();

    static void saveToCache(uint hashCode, const QString &url, qint64 size, int numAccesses);

    void addLocation(const QString &clientId, const QString &dateTime) { m_cacheLocations.addLocation(clientId, dateTime); }
    void addLocation(const QString &clientId, const QDateTime &dateTime) { m_cacheLocations.addLocation(clientId, dateTime); }
    void removeLocation(const QString &clientId) { m_cacheLocations.removeLocation(clientId); }
    bool containsLocation(const QString &clientId) const { return m_cacheLocations.containsLocation(clientId); }
    bool isCacheAvailable() const { return m_cacheLocations.isCacheAvailable(); }

    void downloadFailed();
    void downloadFinished(qint64 size);

signals:
    void readyRead();
    void finished();
    void failed();

private:
    void startDownload(CacheLocations::LocationType locationType, QString clientId);
    void deregisterDependentObject();

    ProxyRequest *m_request;
    ProxyHandlerSession *m_session;

    int m_sessionDependentObjectId;
    uint m_cacheId;

    QMutex m_startedMutex;
    bool m_inProgress;

    CacheLocations m_cacheLocations;
};

#endif // PROXYWEBDOWNLOAD_H
