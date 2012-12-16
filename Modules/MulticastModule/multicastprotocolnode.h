#ifndef COMMUNICATIONINSTANCE_H
#define COMMUNICATIONINSTANCE_H

#include <QObject>
#include <QDateTime>

#include "multicastprotocol.h"

class MulticastProtocolNode : public QObject
{
    Q_OBJECT
public:
    MulticastProtocolNode(const uint &id, QObject *parent = 0);

    static bool lessThan(const MulticastProtocolNode *, const MulticastProtocolNode *);
    void update(uint score, MulticastProtocol::Status status,
                uint port, uint initialized);

    uint id() const;
    uint score() const;
    uint port() const;
    uint initialized() const;
    MulticastProtocol::Status status() const;
    bool isExpired() const;

    void setStatus(MulticastProtocol::Status);
    void setInitialized();

private:

    uint m_id;
    uint m_score;
    uint m_port;
    uint m_initialized;
    MulticastProtocol::Status m_status;
    QDateTime m_lastUpdated;

signals:
    
public slots:
    
};

#endif // COMMUNICATIONINSTANCE_H
