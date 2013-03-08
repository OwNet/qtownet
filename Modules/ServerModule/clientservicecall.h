#ifndef CLIENTSERVICECALL_H
#define CLIENTSERVICECALL_H

#include <QObject>
#include <QNetworkReply>

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
    IResponse *callService(const QString &url, IRequest *request, QNetworkAccessManager *manager);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // CLIENTSERVICECALL_H
