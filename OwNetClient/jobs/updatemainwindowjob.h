#ifndef UPDATEMAINWINDOWJOB_H
#define UPDATEMAINWINDOWJOB_H

#include "job.h"

class UpdateMainWindowJob : public Job
{
public:
    UpdateMainWindowJob(QObject *parent = 0);

    void execute();
};

#endif // UPDATEMAINWINDOWJOB_H
