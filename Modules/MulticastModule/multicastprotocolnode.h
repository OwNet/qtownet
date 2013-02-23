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
                uint port, QString address, uint initialized, QString workspaceId, QString workspaceName);

    uint id() const;
    uint score() const;
    uint port() const;
    QString address() const;
    uint initialized() const;
    MulticastProtocol::Status status() const;
    bool isExpired() const;

    void setStatus(MulticastProtocol::Status);
    void setInitialized();

    QVariantMap message() const;

private:

    uint m_id;
    uint m_score;
    uint m_port;
    QString m_address;
    QString m_workspaceId;
    QString m_workspaceName;
    uint m_initialized;
    MulticastProtocol::Status m_status;
    QDateTime m_lastUpdated;

signals:
    
public slots:
    
};

#endif // COMMUNICATIONINSTANCE_H
