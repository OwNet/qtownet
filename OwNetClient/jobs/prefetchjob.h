#ifndef PREFETCHJOB_H
#define PREFETCHJOB_H


#include "job.h"

#include <QMutex>
class WebPageManager;

class PrefetchJob : public Job
{
public:
    PrefetchJob(QObject *parent = NULL);

protected:
    void execute();

private:
    void prefetch();
    WebPageManager* wpm;
    QMutex m_activeMutex;
};

#endif // PREFETCHJOB_H
