#ifndef PROXYTHREAD_H
#define PROXYTHREAD_H

#include <QObject>

class QTimer;
class ProxyHandlerSession;
class IProxyMockable;
class HttpRequest;
class HttpResponse;

class ProxyHandler : public QObject
{
    Q_OBJECT

    enum {
        Timeout = 15000 // 0.25 min
    };

public:
    ProxyHandler(QObject *parent = NULL);

    void service(HttpRequest *request, HttpResponse *response);

public slots:
    void proxyHandlerSessionFinished();

private slots:
    void requestTimeout();
    void restartTimeout();
    void finishHandling();

private:
    HttpRequest *m_request;
    HttpResponse *m_response;

    ProxyHandlerSession *m_proxyHandlerSession;
    QTimer *m_timeoutTimer;
};

#endif // PROXYTHREAD_H
