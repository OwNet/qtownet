#include "updaterecordssavejob.h"

#include "databaseupdate.h"

UpdateRecordsSaveJob::UpdateRecordsSaveJob(QObject *parent)
    : Job(1 * 1000, parent)
{
}

void UpdateRecordsSaveJob::execute()
{
    DatabaseUpdate::saveLastRecordNumbers();
}
