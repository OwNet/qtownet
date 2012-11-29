#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QVariantMap>

class CommunicationInstance;

class CommunicationManager
{

public:
    enum Status {
        NONE,
        INITIALIZING,
        CLIENT,
        SERVER
    };
    static const int expirationTimeInSeconds;

    static CommunicationManager *getInstance();

    // not private due to unit tests
    CommunicationManager();

    void processMessage(QVariantMap *);
    QList<CommunicationInstance *> &getCommunicationInstances();
    CommunicationInstance *getServer();

    QString myId() const;
    int myScore() const;
    CommunicationManager::Status myStatus();

    void initialized();

private:
    static CommunicationManager *m_communicationManager;

    void cleanAndSortInstances();

    QList<CommunicationInstance *> m_instances;
    QString m_myId;
    int m_myScore;
    bool m_initializing;
};

#endif // COMMUNICATIONMANAGER_H
