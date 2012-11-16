#include <QtWidgets/QApplication>
#include "mainwindow.h"
#include "initializer.h"
#include "proxyrequestbus.h"
#include "applicationenvironment.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    Initializer initializer;
    initializer.init();

    if (!ApplicationEnvironment().contains("OWNET_DISABLE_GUI"))
        w.show();

    return a.exec();
}
