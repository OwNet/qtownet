#include "gdsfclock.h"

#include "qmath.h"
#include "databasesettings.h"

#include <QVariant>

GDSFClock::GDSFClock(QObject *parent) :
    QObject(parent), m_lastClock(0)
{
    m_lastClock = DatabaseSettings().value("cache_clean_clock", QString::number(0.0)).toDouble();
}

double GDSFClock::getGDSFPriority(int accessCount, long size)
{
    return lastClock() + accessCount * (100 / qLn(size + 1.1));
}

double GDSFClock::lastClock()
{
    int clock = 0;
    m_lastClockMutex.lock();
    clock = m_lastClock;
    m_lastClockMutex.unlock();
    return clock;
}

void GDSFClock::setLastClock(double clock)
{
    m_lastClockMutex.lock();
    if (clock < m_lastClock) {
        m_lastClockMutex.unlock();
        return;
    }
    m_lastClock = clock;
    m_lastClockMutex.unlock();

    DatabaseSettings().setValue("cache_clean_clock", QString::number(clock));
}
