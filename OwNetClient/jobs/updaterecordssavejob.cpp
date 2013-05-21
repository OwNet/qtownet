#include "updaterecordssavejob.h"

#include "databaseupdate.h"
#include "webdownloadsmanager.h"

UpdateRecordsSaveJob::UpdateRecordsSaveJob(QObject *parent)
    : Job(1 * 1000, parent)
{
}

void UpdateRecordsSaveJob::execute()
{
    DatabaseUpdate::saveLastRecordNumbers();
    WebDownloadsManager::instance()->saveCacheAccesses();
}
