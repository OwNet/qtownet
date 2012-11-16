#include "proxytrafficcounter.h"

ProxyTrafficCounter::ProxyTrafficCounter(QObject *parent) :
    QObject(parent), m_trafficArrayIndex(0), m_currentTraffic(0)
{
    for (int i = 0; i < TrafficArraySize; ++i)
        m_traffic[i] = 0;
}

void ProxyTrafficCounter::increaseCurrentTraffic()
{
    m_currentTrafficLock.lock();
    m_currentTraffic++;
    m_currentTrafficLock.unlock();
}

void ProxyTrafficCounter::takeCurrentTrafficSnapshot(){
    m_currentTrafficLock.lock();
    m_traffic[m_trafficArrayIndex] = m_currentTraffic;
    m_trafficArrayIndex++;
    m_trafficArrayIndex %= TrafficArraySize;
    m_currentTraffic = 0;
    m_currentTrafficLock.unlock();
}

int ProxyTrafficCounter::lastTraffic(){
    int trafficSum = 0;

    for (int i = 0; i < TrafficArraySize; ++i)
        trafficSum += m_traffic[i];

    return trafficSum;
}
