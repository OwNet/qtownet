#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <QObject>

class Initializer : public QObject
{
    Q_OBJECT

public:
    Initializer(QObject *parent = NULL);

    void init();

private:
    void createPidFile();
};

#endif // INITIALIZER_H
