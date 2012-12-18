#include "multicastprotocol.h"
#include "multicastprotocolnode.h"
#include "iproxyconnection.h"
#include "idatabasesettings.h"
#include "isession.h"

#include <QDebug>
#include <QDateTime>
#include <QMutexLocker>

const int MulticastProtocol::expirationTimeInSeconds = 15;

MulticastProtocol::MulticastProtocol(IProxyConnection *connection, QObject *parent)
    : m_proxyConnection(connection), QObject(parent)
{
    uint myId = connection->databaseSettings(this)->clientId();

    // add self as first instance
    m_currentNode = new MulticastProtocolNode(myId);
    m_currentNode->update(1, INITIALIZING, 8081, "127.0.0.1", 0);
    m_nodes.append(m_currentNode);
}

MulticastProtocolNode *MulticastProtocol::currentNode() const
{
    return m_currentNode;
}

MulticastProtocolNode *MulticastProtocol::serverNode() const
{
    if (m_nodes.count() <= 1)
        return NULL; // no other proxies
    else
    {
        if (m_nodes.first() == m_currentNode)
            return NULL; // this is server
        else if (m_nodes.first()->initialized() == 0)
            return NULL; // future server initializing
        else
            return m_nodes.first(); // best is the server
    }
}

void MulticastProtocol::initialized()
{
    QMutexLocker locker(&m_nodesMutex);

    m_currentNode->setInitialized();
}

void MulticastProtocol::processMessage(QVariantMap *message)
{
    QMutexLocker locker(&m_nodesMutex);

    MulticastProtocolNode *node = NULL;

    uint id = message->value("id").toUInt();
    uint score = message->value("score").toUInt();
    uint port = message->value("port").toUInt();
    uint initialized = message->value("initialized").toUInt();
    QString address = message->value("address").toString();

    Status status;
    if (message->value("status") == "initializing")
        status = INITIALIZING;
    else if (message->value("status") == "client")
        status = CLIENT;
    else if (message->value("status") == "server")
        status = SERVER;

    // find proxy by id
    for (int i = 0; i < m_nodes.size(); ++i)
    {
        if (m_nodes.at(i)->id() == id)
        {
            node = m_nodes.at(i);
            break;
        }
    }

    // or create new
    if (! node)
    {
        node = new MulticastProtocolNode(id);
        m_nodes.append(node);
    }

    // update info
    if (node != m_currentNode)
        node->update(score, status, port, address, initialized);
}

QList<MulticastProtocolNode *> &MulticastProtocol::nodes()
{
    return m_nodes;
}


void MulticastProtocol::update()
{
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    QMutexLocker locker(&m_nodesMutex);

    // clean expired nodes
    QMutableListIterator<MulticastProtocolNode *> i(m_nodes);

    while (i.hasNext())
    {
        MulticastProtocolNode *node = i.next();

        if (node->id() != m_currentNode->id() && node->isExpired())
            i.remove();
    }

    // sort proxies by score
    qSort(m_nodes.begin(), m_nodes.end(), MulticastProtocolNode::lessThan);

    // set current node status
    m_currentNode->setStatus(currentNodesStatus());

    // save server to session
    if (serverNode() != NULL)
        session->setValue("server_id", serverNode()->id());
    else
        session->setValue("server_id", QVariant());

    // save nodes to session
    QVariantMap availableClients;
    for (int i = 0; i < m_nodes.size(); i++) {
        availableClients.insert(QString::number(m_nodes.at(i)->id()), m_nodes.at(i)->address());
    }
    session->setValue("available_clients", availableClients);
}

MulticastProtocol::Status MulticastProtocol::currentNodesStatus() const
{
    if (m_currentNode->initialized() == 0)
        return INITIALIZING; // not initialized yet

    // find first after INITIALIZING
    for (int i = 0; i < m_nodes.size(); ++i)
    {
        if (m_nodes.at(i)->initialized() != 0)
        {
            if (m_nodes.at(i)->id() == m_currentNode->id())
                return SERVER; // top score or still server
            else
                return CLIENT;
        }
    }

    return SERVER;
}

QVariantMap MulticastProtocol::message() const
{
    QVariantMap message;
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
