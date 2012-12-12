#ifndef HEARTBEATSERVER_H
#define HEARTBEATSERVER_H

class QHostAddress;
class QUdpSocket;

#include <QObject>
#include <QVariantMap>

class MulticastProtocol;
class IProxyConnection;

class MulticastServer : public QObject
{
    Q_OBJECT

public:
    explicit MulticastServer(IProxyConnection *connection, QObject *parent = 0);

    void start(QHostAddress *groupAddress, int port);
    void stop();
    
signals:
    
public slots:
    void processPendingDatagrams();

private:

    QUdpSocket *m_udpSocket;
    MulticastProtocol *m_protocol;
    IProxyConnection *m_proxyConnection;
};

#endif // HEARTBEATSERVER_H
