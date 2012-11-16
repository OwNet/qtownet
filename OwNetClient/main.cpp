#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "initializer.h"
#include "proxyrequestbus.h"
#include "applicationenvironment.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (! ApplicationEnvironment().contains("OWNET_DISABLE_GUI"))
    {
        MainWindow w;
        w.show();
    }

    Initializer initializer;
    initializer.init();
    
    return a.exec();
}
