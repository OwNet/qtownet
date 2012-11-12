#include "gdsfclock.h"

#include "qmath.h"

#include <QSqlQuery>
#include <QVariant>

GDSFClock::GDSFClock(QObject *parent) :
    QObject(parent), m_lastClock(0)
{
    QSqlQuery query;
    query.prepare("SELECT value FROM cache_clean_clocks LIMIT 1");

    if (query.exec() && query.next())
        m_lastClock = query.value(0).toDouble();
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

    QSqlQuery query;
    query.prepare("DELETE FROM cache_clean_clocks");
    query.exec();

    query.prepare("INSERT INTO cache_clean_clocks (value) VALUES (:value)");
    query.bindValue(":value", clock);
    query.exec();
}
