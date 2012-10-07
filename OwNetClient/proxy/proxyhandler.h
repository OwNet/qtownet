#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QNetworkReply>

class QSemaphore;

class ProxyHandler : public QObject
{
    Q_OBJECT

public:
    ProxyHandler(QObject *parent = NULL);
    ~ProxyHandler();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    void finishHandlingRequest();

    QTcpSocket * m_socket;
    QSemaphore * m_openSemaphore;
    int m_socketDescriptor;
    bool m_writtenToSocket;

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
    void readReply();
    void error(QNetworkReply::NetworkError);
    void downloadFinished();
    void handleRequest();
};

#endif // PROXYTHREAD_H
