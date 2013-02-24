 #include "jobinitializer.h"

#include "cleancachejob.h"
#include "proxytrafficcounterjob.h"
#include "updaterecordssavejob.h"
#include "dumplogfilejob.h"
#include "updatemainwindowjob.h"

JobInitializer::JobInitializer(QObject *parent)
    : QObject(parent)
{
}

void JobInitializer::init()
{
    new CleanCacheJob(this);
    new ProxyTrafficCounterJob(this);
    new UpdateRecordsSaveJob(this);
    new DumpLogFileJob(this);
    new UpdateMainWindowJob(this);
}
