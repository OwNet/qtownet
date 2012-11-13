#ifndef JOBINITIALIZER_H
#define JOBINITIALIZER_H

class CleanCacheJob;

class JobInitializer
{
public:
    JobInitializer();
    ~JobInitializer();

    void init();

private:
    CleanCacheJob *m_cleanCacheJob;
};

#endif // JOBINITIALIZER_H
