#ifndef PINGSERVER_H
#define PINGSERVER_H

#include <QObject>
#include <QVariantMap>

class MulticastProtocol;
class IProxyConnection;

class PingServer : public QObject
{
    Q_OBJECT
public:
    explicit PingServer(IProxyConnection *proxyConnection, QObject *parent = 0);

    void updateAvailableClients(const QVariantList &clients);
    void updateClient(const QVariantMap &client);
    QVariantList availableClients(const QString &myIp) const;

private:
    MulticastProtocol *m_multicastProtocol;
    IProxyConnection *m_proxyConnection;
};

#endif // PINGSERVER_H
