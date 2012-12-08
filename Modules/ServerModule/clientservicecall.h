#ifndef CLIENTSERVICECALL_H
#define CLIENTSERVICECALL_H

#include <QObject>
#include <QVariant>
#include <QNetworkReply>

class QNetworkAccessManager;
class QSemaphore;
class IRequest;
class IProxyConnection;

class ClientServiceCall : public QObject
{
    Q_OBJECT
public:
    explicit ClientServiceCall(IProxyConnection *proxyConnection, QObject *parent = 0);

    QVariant callClientService(int clientId, IRequest *request);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // CLIENTSERVICECALL_H
