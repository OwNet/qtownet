#ifndef UPDATEMAINWINDOWJOB_H
#define UPDATEMAINWINDOWJOB_H

#include "job.h"

class UpdateMainWindowJob : public Job
{
public:
    UpdateMainWindowJob(QObject *parent = 0);

    void execute();
    bool createSeparateThread() const { return false; }
};

#endif // UPDATEMAINWINDOWJOB_H
