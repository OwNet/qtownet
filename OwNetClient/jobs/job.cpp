#include "job.h"

#include <QTimer>

Job::Job(int interval, QObject *parent) :
    QObject(parent), m_interval(interval)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(execute()));
    m_timer->start(m_interval);
}
