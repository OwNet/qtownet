#include "multicastprotocolnode.h"

#ifdef TEST
#include "stub/stubtime.h"
#endif

MulticastProtocolNode::MulticastProtocolNode(const QString &id, QObject *parent) :
    m_id(id), m_lastUpdated(QDateTime()), QObject(parent)
{
}

bool MulticastProtocolNode::lessThan(const MulticastProtocolNode *first,
                                     const MulticastProtocolNode *second)
{
    return first->score() > second->score();
}

void MulticastProtocolNode::update(int score, MulticastProtocol::Status status)
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

QString MulticastProtocolNode::id() const
{
    return m_id;
}

int MulticastProtocolNode::score() const
{
    return m_score;
}

MulticastProtocol::Status MulticastProtocolNode::status() const
{
    return m_status;
}

bool MulticastProtocolNode::isExpired() const
{
    QDateTime currentDateTime;

#ifdef TEST
    currentDateTime = StubTime::currentDateTime();
#else
    currentDateTime = QDateTime::currentDateTime();
#endif

    return m_lastUpdated.addSecs(MulticastProtocol::expirationTimeInSeconds) < currentDateTime;
}
