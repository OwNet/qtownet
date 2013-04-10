#include "modulejob.h"

#include "ijobaction.h"
#include "proxyconnection.h"

#include <QThread>

ModuleJob::ModuleJob(IJobAction *jobAction, QObject *parent)
    : Job(jobAction->interval(), parent),
      m_jobAction(jobAction)
{
}

void ModuleJob::execute()
{
    m_jobAction->execute();
}

void ModuleJob::start()
{
    m_jobAction->setProxyConnection(new ProxyConnection(this));
    Job::start();
}

bool ModuleJob::createSeparateThread() const
{
    return m_jobAction->createSeparateThread();
}
