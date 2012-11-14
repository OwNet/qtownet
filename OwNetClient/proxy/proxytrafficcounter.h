#ifndef PROXYTRAFFICCOUNTER_H
#define PROXYTRAFFICCOUNTER_H

#include <QObject>
#include <QMutex>

#define TrafficArraySize 60

class ProxyTrafficCounter : public QObject
{
    Q_OBJECT
public:

    explicit ProxyTrafficCounter(QObject *parent = 0);
    void ProxyTraffic();
    void IncreaseCurrentTraffic();
    void TakeCurrentTrafficSnapshot();
    int LastTraffic();

private:

    int m_trafficArrayIndex;
    int m_currentTraffic;
    int m_traffic[TrafficArraySize];
    QMutex m_currentTrafficLock;
signals:
    
public slots:
    
};

#endif // PROXYTRAFFICCOUNTER_H
