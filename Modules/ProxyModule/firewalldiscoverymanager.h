#ifndef FIREWALLDISCOVERYMANAGER_H
#define FIREWALLDISCOVERYMANAGER_H

#include <QObject>

class IProxyConnection;

class FirewallDiscoveryManager : public QObject
{
    Q_OBJECT
public:
    explicit FirewallDiscoveryManager(IProxyConnection *proxyConnection, QObject *parent = 0);

    void ping(const QString &clientId);

    void checkFirewallStatus();
    static void setPingedBack(bool pingedBack);

signals:
    void finishedPing();

private slots:
    void startPing();
    void checkPingResponse();

private:
    QString m_clientToPingId;
    IProxyConnection *m_proxyConnection;
    static bool m_wasPingedBack;
    static bool m_statusChecked;
};

#endif // FIREWALLDISCOVERYMANAGER_H
