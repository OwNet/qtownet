#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QObject>

class QTimer;
class ProxyHandlerSession;

class ProxyHandler : public QObject
{
    Q_OBJECT

    enum {
        Timeout = 30000 // 0.5 min
    };

public:
    ProxyHandler(QObject *parent = NULL);

    void setDescriptorAndStart(int handle);

signals:
    void start(); ///< Signal to start the download in the ProxyHandler thread
    void disposeThread(); ///< Signal to dispose the ProxyHandler thread

public slots:
    void proxyHandlerSessionFinished();

private slots:
    void handleRequest();
    void requestTimeout();
    void restartTimeout();
    void finishHandling();

private:
    int m_socketDescriptor;

    ProxyHandlerSession *m_proxyHandlerSession;
    QTimer *m_timeoutTimer;
};

#endif // PROXYTHREAD_H
