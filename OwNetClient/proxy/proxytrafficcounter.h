#ifndef PROXYTRAFFICCOUNTER_H
#define PROXYTRAFFICCOUNTER_H

#include <QObject>
#include <QMutex>

class ProxyTrafficCounter : public QObject
{
    Q_OBJECT
public:
    enum {
        TrafficArraySize = 60
    };

    explicit ProxyTrafficCounter(QObject *parent = 0);

    void increaseCurrentTraffic();
    void takeCurrentTrafficSnapshot();
    int lastTraffic();

private:
    int m_trafficArrayIndex;
    int m_currentTraffic;
    int m_traffic[TrafficArraySize];
    QMutex m_currentTrafficLock;
};

#endif // PROXYTRAFFICCOUNTER_H
