#ifndef SYNCCLIENT_H
#define SYNCCLIENT_H

#include <QObject>

class IProxyConnection;

class SyncClient : public QObject
{
    Q_OBJECT
public:
    explicit SyncClient(IProxyConnection *proxyConnection, QObject *parent = 0);

    bool updateFromServer();
    int updateFromClients();
    void sync();

private:
    bool downloadUpdatesFromClient(uint clientId);

    IProxyConnection *m_proxyConnection;
};

#endif // SYNCCLIENT_H
