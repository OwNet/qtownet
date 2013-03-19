#ifndef OWNETAPPLICATION_H
#define OWNETAPPLICATION_H

#include <QtWidgets/QApplication>

class OwNetApplication : public QApplication
{
public:
    OwNetApplication(int &argc, char **argv);

    bool notify(QObject *receiver, QEvent *event);
};

#endif // OWNETAPPLICATION_H
