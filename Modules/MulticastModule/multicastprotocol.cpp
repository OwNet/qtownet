#include "multicastprotocol.h"
#include "multicastprotocolnode.h"
#include "iproxyconnection.h"

#include <QDebug>

const int MulticastProtocol::expirationTimeInSeconds = 15;

MulticastProtocol::MulticastProtocol(IProxyConnection *connection, QObject *parent)
    : m_proxyConnection(connection), QObject(parent), m_initializing(true)
{
    m_myId = "unique-client-id";
    m_myScore = 1;

    // add self as first instance
    MulticastProtocolNode *node;
    node = new MulticastProtocolNode(m_myId);
    m_nodeList.append(node);
    node->update(m_myScore, NONE);
}

QString MulticastProtocol::myId() const
{
    return m_myId;
}

int MulticastProtocol::myScore() const
{
    return m_myScore;
}

MulticastProtocol::Status MulticastProtocol::myStatus()
{
    if (m_initializing)
        return INITIALIZING; // not initialized yet

    // find first after INITIALIZING
    for (int i = 0; i < m_nodeList.size(); ++i)
    {
        if (m_nodeList.at(i)->status() != INITIALIZING)
        {
            if (m_nodeList.at(i)->id() == myId())
                return SERVER; // top score or still server
            else
                return CLIENT;
        }
    }

    return SERVER;
}

void MulticastProtocol::initialized()
{
    m_initializing = false;
}

void MulticastProtocol::processMessage(QVariantMap *message)
{
    MulticastProtocolNode *communicationInstance = NULL;

    QString id = message->value("id").toString();
    int score = message->value("score").toInt();

    Status status;
    if (message->value("status") == "initializing")
        status = INITIALIZING;
    else if (message->value("status") == "client")
        status = CLIENT;
    else if (message->value("status") == "server")
        status = SERVER;

    // find proxy by id
    for (int i = 0; i < m_nodeList.size(); ++i)
    {
        if (m_nodeList.at(i)->id() == id)
        {
            communicationInstance = m_nodeList.at(i);
            break;
        }
    }

    // or create new
    if (! communicationInstance)
    {
        communicationInstance = new MulticastProtocolNode(id);
        m_nodeList.append(communicationInstance);
    }

    // update info
    communicationInstance->update(score, status);

    updateNodes();
}

QList<MulticastProtocolNode *> &MulticastProtocol::getNodeList()
{
    return m_nodeList;
}

QMap<QString, MulticastProtocolNode *> &MulticastProtocol::getNodeMap()
{
    return m_nodeMap;
}

MulticastProtocolNode *MulticastProtocol::getServer()
{
    if (m_nodeList.empty())
        return NULL; // no other proxies
    else
    {
        if (m_nodeList.first()->status() == INITIALIZING)
            return NULL; // future server initializing
        else
            return m_nodeList.first(); // best is the server
    }
}

void MulticastProtocol::updateNodes()
{
    // clean expired proxies and build proxy map
    QMutableListIterator<MulticastProtocolNode *> i(m_nodeList);

    m_nodeMap.clear();
    while (i.hasNext())
    {
        MulticastProtocolNode *node;

        node = i.next();

        if (node->isExpired())
            i.remove();
        else
        {
            m_nodeMap.insert(node->id(), node);
        }
    }

    // sort proxies by score
    qSort(m_nodeList.begin(), m_nodeList.end(), MulticastProtocolNode::lessThan);
}
