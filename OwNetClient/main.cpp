#include "ownetapplication.h"
#include "mainwindow.h"
#include "initializer.h"
#include "proxyrequestbus.h"
#include "applicationenvironment.h"
#include <sys/signal.h>

// 24.12.2012: Stastne a vesele prajem :-)

int main(int argc, char *argv[])
{
    OwNetApplication a(argc, argv);
    signal(SIGPIPE, SIG_IGN);

    MainWindow *w = NULL;

    Initializer initializer;
    initializer.init();

    if (! ApplicationEnvironment().contains("OWNET_TEST_ENVIRONMENT"))
    {
        w = new MainWindow;
        w->show();
    }

    return a.exec();
}
