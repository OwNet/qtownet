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
        Unknown
    };

    explicit ProxyWebDownload(ProxyRequest *request, QObject *parent = 0);
    
    QIODevice *getStream(ProxyWebReader *reader, ProxyHandlerSession *session);

signals:
    void readyRead();
    void finished();

private slots:
    void downloadReadyRead();
    void downloadFinished();
    void downloadFailed();

private:
    void startDownload();

    QString m_url;
    ProxyRequest *m_request;
    ProxyHandlerSession *m_session;

    Type m_type;
    int m_sessionDependentObjectId;

    QMutex m_startedMutex;
    bool m_started;
    bool m_failed;

    QFile *m_writeFile;
    WebSocket *m_socket;
};

#endif // PROXYWEBDOWNLOAD_H
