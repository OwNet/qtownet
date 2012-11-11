#ifndef JOB_H
#define JOB_H

class QTimer;

#include <QObject>

class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(int interval, QObject *parent = 0);

protected slots:
    virtual void execute() = 0;

private:
    int m_interval;
    QTimer *m_timer;
};

#endif // JOB_H
