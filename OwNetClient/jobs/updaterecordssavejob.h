#ifndef UPDATERECORDSSAVEJOB_H
#define UPDATERECORDSSAVEJOB_H

#include "job.h"

class UpdateRecordsSaveJob : public Job
{
public:
    UpdateRecordsSaveJob(QObject *parent = 0);

    void execute();
};

#endif // UPDATERECORDSSAVEJOB_H
