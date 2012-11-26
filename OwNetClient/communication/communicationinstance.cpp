#include "communicationinstance.h"

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
    m_lastUpdated = QDateTime::currentDateTime();
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
    return m_lastUpdated.addSecs(10) < QDateTime::currentDateTime();
}
