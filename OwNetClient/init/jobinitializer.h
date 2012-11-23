#ifndef JOBINITIALIZER_H
#define JOBINITIALIZER_H

class CleanCacheJob;
class ProxyTrafficCounterJob;
class HeartbeatJob;

class JobInitializer
{
public:
    JobInitializer();
    ~JobInitializer();

    void init();

private:
    CleanCacheJob *m_cleanCacheJob;
    ProxyTrafficCounterJob *m_trafficCounterJob;
    HeartbeatJob *m_heartbeatJob;
};

#endif // JOBINITIALIZER_H
