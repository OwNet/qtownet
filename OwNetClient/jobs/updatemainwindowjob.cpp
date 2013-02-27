#include "updatemainwindowjob.h"

#ifndef TEST
#include "mainwindow.h"
#endif

UpdateMainWindowJob::UpdateMainWindowJob(QObject *parent)
    : Job(1 * 1000, parent)
{
}

void UpdateMainWindowJob::execute()
{
#ifndef TEST
    if (MainWindow::instance())
        MainWindow::instance()->update();
#endif
}
