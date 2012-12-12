#ifndef COMMUNICATIONINSTANCE_H
#define COMMUNICATIONINSTANCE_H

#include <QObject>
#include <QDateTime>

#include "multicastprotocol.h"

class MulticastProtocolNode : public QObject
{
    Q_OBJECT
public:
    MulticastProtocolNode(const QString &id, QObject *parent = 0);

    static bool lessThan(const MulticastProtocolNode *, const MulticastProtocolNode *);
    void update(int score, MulticastProtocol::Status status);

    QString id() const;
    int score() const;
    MulticastProtocol::Status status() const;
    bool isExpired() const;

private:

    QString m_id;
    int m_score;
    MulticastProtocol::Status m_status;
    QDateTime m_lastUpdated;

signals:
    
public slots:
    
};

#endif // COMMUNICATIONINSTANCE_H
