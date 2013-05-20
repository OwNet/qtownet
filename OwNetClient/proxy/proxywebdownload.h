#ifndef PROXYWEBDOWNLOAD_H
#define PROXYWEBDOWNLOAD_H

#include <QObject>
#include <QMutex>
#include <QAbstractSocket>

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

    ProxyWebDownload(ProxyRequest *request, QObject *parent = 0);
    ProxyWebDownload(uint cacheId, const QString &clientId, QObject *parent = 0);
    
    QIODevice *getStream(ProxyWebReader *reader, ProxyHandlerSession *session, bool *finished);

    bool isLocalCache() const { return m_type == CacheFile; }
    bool isWeb() const { return m_type == WebStream; }
    bool isNetworkCache() const { return m_type == NetworkStream; }

    QString clientId() const { return m_clientId; }
    bool exists();

    void downloadFromTheWebOrNetwork(ProxyRequest *request, const QString clientId = QString());
    static void saveToCache(uint hashCode, const QString &url, qint64 size, int numAccesses);

signals:
    void readyRead();
    void finished();
    void failed();

private slots:
    void downloadFailed();
    void downloadFinished(qint64 size);
    void deregisterDependentObject();

private:
    void startDownload();

    QString m_clientId;
    ProxyRequest *m_request;
    ProxyHandlerSession *m_session;

    Type m_type;
    int m_sessionDependentObjectId;
    uint m_cacheId;

    QMutex m_startedMutex;
    bool m_started;
    bool m_failed;
    bool m_finished;
};

#endif // PROXYWEBDOWNLOAD_H
