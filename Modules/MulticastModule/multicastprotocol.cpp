#include "multicastprotocol.h"
#include "multicastprotocolnode.h"
#include "iproxyconnection.h"
#include "idatabasesettings.h"

#include <QDebug>
#include <QDateTime>

const int MulticastProtocol::expirationTimeInSeconds = 15;

MulticastProtocol::MulticastProtocol(IProxyConnection *connection, QObject *parent)
    : m_proxyConnection(connection), QObject(parent)
{
    uint myId = connection->databaseSettings(this)->clientId();

    // add self as first instance
    m_currentNode = new MulticastProtocolNode(myId);
    m_currentNode->update(1, INITIALIZING, 8081, 0);
    m_nodeList.append(m_currentNode);
}

MulticastProtocolNode *MulticastProtocol::currentNode()
{
    updateNodes();

    return m_currentNode;
}

MulticastProtocolNode *MulticastProtocol::serverNode()
{
    updateNodes();

    if (m_nodeList.count() <= 1)
        return NULL; // no other proxies
    else
    {
        if (m_nodeList.first()->status() == INITIALIZING)
            return NULL; // future server initializing
        else
            return m_nodeList.first(); // best is the server
    }
}

void MulticastProtocol::initialized()
{
    m_currentNode->setInitialized(QDateTime::currentDateTime().toString(Qt::ISODate).toUInt());
}

void MulticastProtocol::processMessage(QVariantMap *message)
{
    MulticastProtocolNode *node = NULL;

    uint id = message->value("id").toUInt();
    uint score = message->value("score").toUInt();
    uint port = message->value("port").toUInt();
    uint initialized = message->value("initialized").toUInt();

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
            node = m_nodeList.at(i);
            break;
        }
    }

    // or create new
    if (! node)
    {
        node = new MulticastProtocolNode(id);
        m_nodeList.append(node);
    }

    // update info
    if (node != m_currentNode)
        node->update(score, status, port, initialized);
}

QList<MulticastProtocolNode *> &MulticastProtocol::nodeList()
{
    updateNodes();

    return m_nodeList;
}

QMap<uint, MulticastProtocolNode *> &MulticastProtocol::nodeMap()
{
    updateNodes();

    return m_nodeMap;
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

        if (node->id() != m_currentNode->id() && node->isExpired())
            i.remove();
        else
        {
            m_nodeMap.insert(node->id(), node);
        }
    }

    // sort proxies by score
    qSort(m_nodeList.begin(), m_nodeList.end(), MulticastProtocolNode::lessThan);

    // se status
    m_currentNode->setStatus(currentNodesStatus());
}

MulticastProtocol::Status MulticastProtocol::currentNodesStatus() const
{
    if (m_currentNode->initialized() == 0)
        return INITIALIZING; // not initialized yet

    // find first after INITIALIZING
    for (int i = 0; i < m_nodeList.size(); ++i)
    {
        if (m_nodeList.at(i)->status() != INITIALIZING)
        {
            if (m_nodeList.at(i)->id() == m_currentNode->id())
                return SERVER; // top score or still server
            else
                return CLIENT;
        }
    }

    return SERVER;
}

QVariantMap MulticastProtocol::message()
{
    QVariantMap message;

    updateNodes();

    QString status;
    switch (m_currentNode->status())
    {
    case MulticastProtocol::INITIALIZING:
        status = "initializing";
        break;
    case MulticastProtocol::CLIENT:
        status = "client";
        break;
    case MulticastProtocol::SERVER:
        status = "server";
        break;
    }

    message.insert("id", m_currentNode->id());
    message.insert("score", m_currentNode->score());
    message.insert("status", status);
    message.insert("port", 8081);
    message.insert("initialized", m_currentNode->initialized());

    return message;
}
