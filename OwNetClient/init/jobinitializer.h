#ifndef JOBINITIALIZER_H
#define JOBINITIALIZER_H

#include <QObject>

class Job;

class JobInitializer : public QObject
{
public:
    JobInitializer(QObject *parent = NULL);

    void init();
    static QThread *startJob(Job *job);
};

#endif // JOBINITIALIZER_H
