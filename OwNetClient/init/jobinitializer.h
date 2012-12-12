#ifndef JOBINITIALIZER_H
#define JOBINITIALIZER_H

class CleanCacheJob;
class ProxyTrafficCounterJob;
class HeartbeatJob;
class UpdateRecordsSaveJob;

class JobInitializer
{
public:
    JobInitializer();
    ~JobInitializer();

    void init();

private:
    CleanCacheJob *m_cleanCacheJob;
    ProxyTrafficCounterJob *m_trafficCounterJob;
    UpdateRecordsSaveJob *m_updateRecordsSaveJob;
};

#endif // JOBINITIALIZER_H
