#include "modulejob.h"

#include "ijobaction.h"

ModuleJob::ModuleJob(IJobAction *jobAction, QObject *parent)
    : Job(jobAction->interval(), parent),
      m_jobAction(jobAction)
{
}

void ModuleJob::execute()
{
    m_jobAction->execute();
}
