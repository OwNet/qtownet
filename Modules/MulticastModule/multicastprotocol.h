#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QVariantMap>
#include <QDateTime>
#include <QMutex>

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
    QList<MulticastProtocolNode *> &nodes();
    MulticastProtocolNode *serverNode() const;
    MulticastProtocolNode *currentNode() const;

    QVariantMap message() const;

    void initialized();
    void update();

private:
    MulticastProtocol::Status currentNodesStatus() const;

    MulticastProtocolNode *m_currentNode;
    QList<MulticastProtocolNode *> m_nodes;

    IProxyConnection *m_proxyConnection;

    QMutex m_nodesMutex;
};

#endif // COMMUNICATIONMANAGER_H
