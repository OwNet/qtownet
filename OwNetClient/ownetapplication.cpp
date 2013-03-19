#include "ownetapplication.h"

#include "messagehelper.h"

OwNetApplication::OwNetApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{}

bool OwNetApplication::notify(QObject *receiver, QEvent *event)
{
    bool done = true;
    try {
        done = QApplication::notify(receiver, event);
    } catch (const std::exception &ex) {
        MessageHelper::debug(QString("Caught exception in QApplication: %1").arg(ex.what()));
    }
    return done;
}
