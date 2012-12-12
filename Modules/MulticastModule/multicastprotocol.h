#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QVariantMap>
#include <QDateTime>

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
    QMap<uint, MulticastProtocolNode *> &getNodeMap();
    MulticastProtocolNode *getServer();

    uint myId() const;
    uint myScore() const;
    MulticastProtocol::Status myStatus();

    QVariantMap getMessage();

    void initialized();

private:
    void updateNodes();

    QList<MulticastProtocolNode *> m_nodeList;
    QMap<uint, MulticastProtocolNode *> m_nodeMap;

    uint m_myId;
    uint m_myScore;
    QDateTime m_initialized;

    IProxyConnection *m_proxyConnection;
};

#endif // COMMUNICATIONMANAGER_H
