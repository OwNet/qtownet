#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QVariantMap>

class IProxyConnection;

class MulticastProtocolNode;

class MulticastProtocol : public QObject
{
    Q_OBJECT

public:
    enum Status {
        NONE,
        INITIALIZING,
        CLIENT,
        SERVER
    };
    static const int expirationTimeInSeconds;

    MulticastProtocol(IProxyConnection *connection, QObject *parent = 0);

    void processMessage(QVariantMap *);
    QList<MulticastProtocolNode *> &getNodeList();
    QMap<QString, MulticastProtocolNode *> &getNodeMap();
    MulticastProtocolNode *getServer();

    QString myId() const;
    int myScore() const;
    MulticastProtocol::Status myStatus();

    void initialized();

private:
    void updateNodes();

    QList<MulticastProtocolNode *> m_nodeList;
    QMap<QString, MulticastProtocolNode *> m_nodeMap;

    QString m_myId;
    int m_myScore;
    bool m_initializing;

    IProxyConnection *m_proxyConnection;
};

#endif // COMMUNICATIONMANAGER_H
