#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QThread>
#include <QTcpSocket>

class ProxyThread : public QThread
{
    Q_OBJECT

public:
    ProxyThread(int socketDescriptor, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int m_socketDescriptor;
};

#endif // PROXYTHREAD_H
