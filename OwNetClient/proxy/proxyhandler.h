#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QNetworkReply>

class QSemaphore;
class ProxyInputObject;
class ProxyRequest;
class QTimer;

class ProxyHandler : public QObject
{
    Q_OBJECT

    enum {
        Timeout = 30000
    };

public:
    ProxyHandler(QObject *parent = NULL);
    ~ProxyHandler();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    void finishHandlingRequest();

    QTcpSocket *m_socket;
    QSemaphore *m_openSemaphore;
    ProxyRequest *m_request;
    int m_socketDescriptor;
    bool m_writtenToSocket;
    QTimer *m_timeoutTimer;

signals:
    void finished();
    void requestFinished(ProxyHandler *);
    void start();

public:
    void setDescriptorAndStart(int handle);

public slots:
    void finish();

private slots:
    void readRequest();
    void readReply(QIODevice *ioDevice, ProxyInputObject *inputObject);
    void error(QNetworkReply::NetworkError);
    void downloadFinished();
    void handleRequest();
    void requestTimeout();
};

#endif // PROXYTHREAD_H
