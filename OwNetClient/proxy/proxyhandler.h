#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QNetworkReply>
#include <QMutex>

class QSemaphore;
class QTimer;
class ProxyInputObject;
class ProxyRequest;
class ProxyDownloads;
class ProxySocketOutputWriter;

class ProxyHandler : public QObject
{
    Q_OBJECT

    enum {
        Timeout = 60000
    };

public:
    ProxyHandler(int handlerId, QObject *parent = NULL);
    ~ProxyHandler();

    void setDescriptorAndStart(int handle);
    void triggerFinish();

    int registerDependentObject();
    void deregisterDependentObject(int objectId);
    bool hasDependentObjects();

    bool isActive() { return m_isActive; }
    int handlerId() { return m_handlerId; }

signals:
    void error(QTcpSocket::SocketError socketError);
    void finished();
    void requestFinished(ProxyHandler *);
    void start();
    void finish();
    void canBeDisposed(ProxyHandler *);

private slots:
    void error(QNetworkReply::NetworkError);
    void handleRequest();
    void requestTimeout();
    void restartTimeout();
    void downloadFinished();
    void finishHandling();

private:
    void finishHandlingRequest();

    int m_socketDescriptor;
    int m_handlerId;
    bool m_isActive;

    QList<int> m_dependentObjects;
    int m_lastDependentObjectId;
    QMutex m_dependentObjectsMutex;

    ProxySocketOutputWriter *m_socketOutputWriter;
    QSemaphore *m_openSemaphore;
    QTimer *m_timeoutTimer;
};

#endif // PROXYTHREAD_H
