#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QObject>

class ProxyHandlerSession;
class IProxyMockable;
class SocketHandler;

class ProxyHandler : public QObject
{
    Q_OBJECT

    enum {
        Timeout = 15000 // 0.25 min
    };

public:
    ProxyHandler(QObject *parent = NULL);

    void service(SocketHandler *socketHandler);

public slots:
    void proxyHandlerSessionFinished();

private slots:
    void finishHandling();

private:
    SocketHandler *m_socketHandler;

    ProxyHandlerSession *m_proxyHandlerSession;
};

#endif // PROXYTHREAD_H
