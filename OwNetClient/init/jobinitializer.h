#ifndef JOBINITIALIZER_H
#define JOBINITIALIZER_H

class CleanCacheJob;
class ProxyTrafficCounterJob;
class MulticastJob;
class UpdateRecordsSaveJob;
class DumpLogFileJob;

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
    DumpLogFileJob *m_dumpLogFileJob;
};

#endif // JOBINITIALIZER_H
