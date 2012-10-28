#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QNetworkReply>

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
    ProxyHandler(QObject *parent = NULL);
    ~ProxyHandler();

    void setDescriptorAndStart(int handle);
    void triggerFinish();

signals:
    void error(QTcpSocket::SocketError socketError);
    void finished();
    void requestFinished(ProxyHandler *);
    void start();
    void finish();

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
    bool m_writtenToSocket;

    ProxySocketOutputWriter *m_socketOutputWriter;
    QSemaphore *m_openSemaphore;
    QTimer *m_timeoutTimer;
};

#endif // PROXYTHREAD_H
