#ifndef GDSFCLOCK_H
#define GDSFCLOCK_H

#include <QObject>
#include <QMutex>

class GDSFClock : public QObject
{
    Q_OBJECT
public:
    explicit GDSFClock(QObject *parent = 0);

    double getGDSFPriority(int accessCount, long size);
    double lastClock();
    void setLastClock(double clock);
    
private:
    double m_lastClock;
    QMutex m_lastClockMutex;
};

#endif // GDSFCLOCK_H
