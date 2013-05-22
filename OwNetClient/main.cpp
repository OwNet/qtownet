#include "ownetapplication.h"
#include "mainwindow.h"
#include "initializer.h"
#include "applicationenvironment.h"
#ifndef Q_OS_WIN
#include <sys/signal.h>
#endif

// 24.12.2012: Stastne a vesele prajem :-)

int main(int argc, char *argv[])
{
    OwNetApplication a(argc, argv);
#ifndef Q_OS_WIN
    signal(SIGPIPE, SIG_IGN);
#endif

    MainWindow *w = NULL;

    Initializer initializer;
    initializer.init();

    if (!ApplicationEnvironment().contains("OWNET_TEST_ENVIRONMENT") && !ApplicationEnvironment().contains("NO_GUI"))
    {
        w = new MainWindow;
        w->show();
    }

    return a.exec();
}
