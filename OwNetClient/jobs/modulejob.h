#ifndef MODULEJOB_H
#define MODULEJOB_H

#include "job.h"

class IJobAction;

class ModuleJob : public Job
{
public:
    ModuleJob(IJobAction *jobAction, QObject *parent = 0);

    void execute();

private:
    IJobAction *m_jobAction;
};

#endif // MODULEJOB_H
