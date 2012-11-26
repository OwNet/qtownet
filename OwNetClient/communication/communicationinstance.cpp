#include "communicationinstance.h"

#ifdef TEST
#include "stubtime.h"
#endif

CommunicationInstance::CommunicationInstance(const QString &id, QObject *parent) :
    m_id(id), m_lastUpdated(QDateTime()), QObject(parent)
{
}

bool CommunicationInstance::lessThan(const CommunicationInstance *first,
                                     const CommunicationInstance *second)
{
    return first->score() > second->score();
}

void CommunicationInstance::update(int score, CommunicationManager::Status status)
{
    m_score = score;
    m_status = status;

    QDateTime currentDateTime;

#ifdef TEST
    currentDateTime = StubTime::currentDateTime();
#else
    currentDateTime = QDateTime::currentDateTime();
#endif

    m_lastUpdated = currentDateTime;
}

QString CommunicationInstance::id() const
{
    return m_id;
}

int CommunicationInstance::score() const
{
    return m_score;
}

CommunicationManager::Status CommunicationInstance::status() const
{
    return m_status;
}

bool CommunicationInstance::isExpired() const
{
    QDateTime currentDateTime;

#ifdef TEST
    currentDateTime = StubTime::currentDateTime();
#else
    currentDateTime = QDateTime::currentDateTime();
#endif

    return m_lastUpdated.addSecs(CommunicationManager::expirationTimeInSeconds) < currentDateTime;
}
