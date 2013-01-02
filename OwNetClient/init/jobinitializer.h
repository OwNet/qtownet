#ifndef JOBINITIALIZER_H
#define JOBINITIALIZER_H

#include <QObject>

class JobInitializer : public QObject
{
public:
    JobInitializer(QObject *parent = NULL);

    void init();
};

#endif // JOBINITIALIZER_H
