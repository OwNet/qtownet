#ifndef CLIENTSERVICECALL_H
#define CLIENTSERVICECALL_H

#include <QObject>
#include <QNetworkReply>
#include <QSemaphore>

class QNetworkAccessManager;
class QSemaphore;
class IRequest;
class IResponse;
class IProxyConnection;

class ClientServiceCall : public QObject
{
    Q_OBJECT
public:
    explicit ClientServiceCall(IProxyConnection *proxyConnection, QObject *parent = 0);

    IResponse *callClientService(const QString &clientId, const QString &apiUrl, IRequest *request);
    IResponse *callClientService(const QString &ip, int port, const QString &apiUrl, IRequest *request);
    IResponse *callCentralService(const QString &relativeUrl, IRequest *request);
    IResponse *callService(const QString &url, IRequest *request, ClientServiceCall *clientServiceCall = NULL);

    void setProxy(const QString &address, int port);
    void setUrl(const QString &url) { m_url = url; }
    void setRequest(IRequest *request) { m_request = request; }

    IResponse *response() { return m_response; }

    void waitForRequest() { m_semaphore.acquire(); }

public slots:
    void startRequest();

private slots:
    void readyRead();

signals:
    void requestFinished();

private:
    IProxyConnection *m_proxyConnection;
    QNetworkAccessManager *m_networkAccessManager;
    QString m_url;
    IRequest *m_request;
    IResponse *m_response;
    QNetworkReply *m_reply;
    QSemaphore m_semaphore;
    QString m_proxyAddress;
    int m_proxyPort;
};

#endif // CLIENTSERVICECALL_H
