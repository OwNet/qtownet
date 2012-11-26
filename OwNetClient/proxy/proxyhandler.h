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
class ProxyHandlerSession;

class ProxyHandler : public QObject
{
    Q_OBJECT

    enum {
        Timeout = 30000 // 0.5 min
    };

public:
    ProxyHandler(int handlerId, QObject *parent = NULL);

    void setDescriptorAndStart(int handle);

    int handlerId() { return m_handlerId; }
    void dispose();

signals:
    void requestFinished(ProxyHandler *);
    void start();
    void disposeThread();

public slots:
    void proxyHandlerSessionFinished();

private slots:
    void handleRequest();
    void requestTimeout();
    void restartTimeout();
    void finishHandling();

private:
    int m_socketDescriptor;
    int m_handlerId;

    ProxyHandlerSession *m_proxyHandlerSession;
    QTimer *m_timeoutTimer;
};

#endif // PROXYTHREAD_H
