#ifndef COMMUNICATIONINSTANCE_H
#define COMMUNICATIONINSTANCE_H

#include <QObject>
#include <QDateTime>

#include "communicationmanager.h"

class CommunicationInstance : public QObject
{
    Q_OBJECT
public:
    CommunicationInstance(const QString &id, QObject *parent = 0);

    static bool lessThan(const CommunicationInstance *, const CommunicationInstance *);
    void update(int score, CommunicationManager::Status status);

    QString id() const;
    int score() const;
    CommunicationManager::Status status() const;
    bool isExpired() const;

private:

    QString m_id;
    int m_score;
    CommunicationManager::Status m_status;
    QDateTime m_lastUpdated;

signals:
    
public slots:
    
};

#endif // COMMUNICATIONINSTANCE_H
