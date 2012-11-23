#ifndef HEARTBEATSERVER_H
#define HEARTBEATSERVER_H

class QHostAddress;
class QUdpSocket;

#include <QObject>
#include <QVariantMap>

class HeartbeatServer : public QObject
{
    Q_OBJECT

public:
    static HeartbeatServer *getInstance();
    void start(QHostAddress *, int);
    void stop();
    
signals:
    
public slots:
    void processPendingDatagrams();

private:
    static HeartbeatServer *m_heartbeatServer;

    explicit HeartbeatServer();
    void processMessage(QVariantMap &message);

    QUdpSocket *m_udpSocket;
};

#endif // HEARTBEATSERVER_H
