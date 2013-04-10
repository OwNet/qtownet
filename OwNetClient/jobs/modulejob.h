#ifndef MODULEJOB_H
#define MODULEJOB_H

#include "job.h"

class IJobAction;

class ModuleJob : public Job
{
public:
    ModuleJob(IJobAction *jobAction, QObject *parent = 0);

    void execute();
    void start();
    bool createSeparateThread() const;

private:
    IJobAction *m_jobAction;
};

#endif // MODULEJOB_H
