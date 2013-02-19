#include "multicastprotocolnode.h"

#ifdef TEST
#include "stub/stubtime.h"
#endif

MulticastProtocolNode::MulticastProtocolNode(const uint &id, QObject *parent) :
    m_id(id), m_lastUpdated(QDateTime()), QObject(parent)
{
}

bool MulticastProtocolNode::lessThan(const MulticastProtocolNode *first,
                                     const MulticastProtocolNode *second)
{
    if (first->score() != second->score())
        return first->score() > second->score();                // descending by score
    else
        return first->initialized() < second->initialized();    // ascending by initialized
}

void MulticastProtocolNode::update(uint score, MulticastProtocol::Status status,
                                   uint port, QString address, uint initialized)
{
    m_score = score;
    m_status = status;
    m_port = port;
    m_address = address;
    m_initialized = initialized;

    // last update
    QDateTime currentDateTime;
#ifdef TEST
    currentDateTime = StubTime::currentDateTime();
#else
    currentDateTime = QDateTime::currentDateTime();
#endif
    m_lastUpdated = currentDateTime;
}

void MulticastProtocolNode::setStatus(MulticastProtocol::Status status)
{
    m_status = status;
}

void MulticastProtocolNode::setInitialized()
{
    QDateTime currentDateTime;
#ifdef TEST
    currentDateTime = StubTime::currentDateTime();
#else
    currentDateTime = QDateTime::currentDateTime();
#endif
    m_initialized = currentDateTime.toTime_t();
}

QVariantMap MulticastProtocolNode::message() const
{
    QVariantMap message;
    QString s;

    switch (status())
    {
    case MulticastProtocol::INITIALIZING:
        s = "initializing";
        break;
    case MulticastProtocol::CLIENT:
        s = "client";
        break;
    case MulticastProtocol::SERVER:
        s = "server";
        break;
    }

    message.insert("id", id());
    message.insert("score", score());
    message.insert("status", s);
    message.insert("port", port());
    message.insert("initialized", initialized());

    return message;
}

uint MulticastProtocolNode::id() const
{
    return m_id;
}

uint MulticastProtocolNode::score() const
{
    return m_score;
}

uint MulticastProtocolNode::port() const
{
    return m_port;
}

QString MulticastProtocolNode::address() const
{
    return m_address;
}

uint MulticastProtocolNode::initialized() const
{
    return m_initialized;
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
