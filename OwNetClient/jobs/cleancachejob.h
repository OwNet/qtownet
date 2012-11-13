#ifndef CLEANCACHEJOB_H
#define CLEANCACHEJOB_H

#include "job.h"

#include <QMutex>

class CleanCacheJob : public Job
{
public:
    CleanCacheJob(QObject *parent = NULL);

protected:
    void execute();

private:
    void clean();

    QMutex m_activeMutex;
};

#endif // CLEANCACHEJOB_H
