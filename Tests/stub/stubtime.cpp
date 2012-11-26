#include "stubtime.h"

QDateTime StubTime::m_currentDateTime = QDateTime();

void StubTime::cleanCurrentDateTime()
{
    m_currentDateTime = QDateTime();
}

void StubTime::setCurrentDateTime(QDateTime dt)
{
    m_currentDateTime = dt;
}

QDateTime StubTime::currentDateTime()
{
    if (m_currentDateTime.isValid())
        return m_currentDateTime;
    return
        QDateTime::currentDateTime();
}

void StubTime::addSecs(int secs)
{
   m_currentDateTime = m_currentDateTime.addSecs(secs);
}
