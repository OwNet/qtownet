#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "initializer.h"
#include "proxyrequestbus.h"
#include "applicationenvironment.h"

// 24.12.2012: Stastne a vesele prajem :-)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);	

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
