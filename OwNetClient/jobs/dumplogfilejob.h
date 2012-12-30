#ifndef DUMPLOGFILEJOB_H
#define DUMPLOGFILEJOB_H

#include "job.h"

#include <QMutex>

class DumpLogFileJob : public Job
{
public:
    DumpLogFileJob(QObject *parent = NULL);

    void execute();

private:
    QMutex m_activeMutex;
};

#endif // DUMPLOGFILEJOB_H
