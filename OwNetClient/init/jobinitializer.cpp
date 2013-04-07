 #include "jobinitializer.h"

#include "cleancachejob.h"
#include "proxytrafficcounterjob.h"
#include "updaterecordssavejob.h"
#include "dumplogfilejob.h"
#include "updatemainwindowjob.h"

#include <QThread>

JobInitializer::JobInitializer(QObject *parent)
    : QObject(parent)
{
}

void JobInitializer::init()
{
    startJob(new CleanCacheJob);
    startJob(new ProxyTrafficCounterJob);
    startJob(new UpdateRecordsSaveJob);
    startJob(new DumpLogFileJob);
    startJob(new UpdateMainWindowJob);
}

QThread *JobInitializer::startJob(Job *job)
{
    QThread* thread = new QThread;
    job->moveToThread(thread);
    connect(thread, SIGNAL(started()), job, SLOT(start()));
    connect(job, SIGNAL(finish()), thread, SLOT(terminate()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
    return thread;
}
