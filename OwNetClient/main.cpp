#include <QApplication>
#include "mainwindow.h"
#include "initializer.h"
#include "proxyrequestbus.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Initializer initializer;

    w.show();
    initializer.init();
    
    return a.exec();
}
