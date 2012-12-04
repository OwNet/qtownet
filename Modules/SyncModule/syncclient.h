#ifndef SYNCCLIENT_H
#define SYNCCLIENT_H

#include <QObject>

class IProxyConnection;

class SyncClient : public QObject
{
    Q_OBJECT
public:
    explicit SyncClient(IProxyConnection *proxyConnection, QObject *parent = 0);

    void update();

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SYNCCLIENT_H
