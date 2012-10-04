#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QNetworkReply>

class QMutex;

class ProxyThread : public QThread
{
    Q_OBJECT

public:
    ProxyThread(QTcpSocket * socket, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    void end();

    QTcpSocket * m_socket;
    int m_socketDescriptor;
    bool m_writtenToSocket;

    QMutex * m_outputMutex;

private slots:
    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError);
    void slotDownloadFinished();
    void slotStarted();
};

#endif // PROXYTHREAD_H
