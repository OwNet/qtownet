#include "gdsfclock.h"

#include "qmath.h"
#include "databaseupdate.h"

#include <QVariant>
#include <QSqlQuery>

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

    DatabaseUpdate update(false);
    update.createUpdateQuery("cache_clean_clocks", DatabaseUpdateQuery::Delete);
    DatabaseUpdateQuery *query = update.createUpdateQuery("cache_clean_clocks", DatabaseUpdateQuery::Insert);
    query->setColumnValue("value", clock);

    update.execute();
}
