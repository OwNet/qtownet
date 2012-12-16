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
        INITIALIZING,
        CLIENT,
        SERVER
    };
    static const int expirationTimeInSeconds;

    MulticastProtocol(IProxyConnection *connection, QObject *parent = 0);

    void processMessage(QVariantMap *);
    QList<MulticastProtocolNode *> &nodeList();
    QMap<uint, MulticastProtocolNode *> &nodeMap();
    MulticastProtocolNode *serverNode();
    MulticastProtocolNode *currentNode();

    QVariantMap message();

    void initialized();

private:
    void updateNodes();
    MulticastProtocol::Status currentNodesStatus() const;

    MulticastProtocolNode *m_currentNode;
    QList<MulticastProtocolNode *> m_nodeList;
    QMap<uint, MulticastProtocolNode *> m_nodeMap;

    IProxyConnection *m_proxyConnection;
};

#endif // COMMUNICATIONMANAGER_H
