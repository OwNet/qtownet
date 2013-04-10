#ifndef JOB_H
#define JOB_H

class QTimer;

#include <QObject>

class Job : public QObject
{
    Q_OBJECT
public:
    explicit Job(int interval, QObject *parent = 0);
    virtual bool createSeparateThread() const { return true; }

public slots:
    virtual void start();

protected slots:
    virtual void execute() = 0;

signals:
    void finish();

private:
    int m_interval;
    QTimer *m_timer;
};

#endif // JOB_H
