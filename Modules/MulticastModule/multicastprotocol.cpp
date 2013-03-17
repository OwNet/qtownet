#include "multicastprotocol.h"
#include "multicastprotocolnode.h"
#include "iproxyconnection.h"
#include "idatabasesettings.h"
#include "isession.h"

#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QMutexLocker>
#include <QProcessEnvironment>

const int MulticastProtocol::expirationTimeInSeconds = 15;

MulticastProtocol::MulticastProtocol(IProxyConnection *connection, QObject *parent)
    : m_proxyConnection(connection), QObject(parent)
{
    QObject parentObject;
    QSettings *settings = m_proxyConnection->settings(&parentObject);

    QString myId = connection->databaseSettings(this)->clientId();
    int port = settings->value("application/listen_port", 8081).toInt();

    // add self as first instance
    m_currentNode = new MulticastProtocolNode(myId, m_proxyConnection);
    m_currentNode->update(1, INITIALIZING, port, "127.0.0.1", 0);
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

void MulticastProtocol::processMessage(const QVariantMap &message)
{
    if (message.value("address").toString().isEmpty())
        return;

    QMutexLocker locker(&m_nodesMutex);

    QObject parent;
    QSettings *settings = m_proxyConnection->settings(&parent);

    QString workspaceId = message.value("workspace_id").toString();
    QString workspaceName = message.value("workspace_name").toString();

    if (workspaceId == settings->value("current_workspace/id")) { // Same as current workspace
        MulticastProtocolNode *node = NULL;
        QString id = message.value("id").toString();
        uint score = message.value("score").toUInt();
        uint port = message.value("port").toUInt();
        uint initialized = message.value("initialized").toUInt();
        QString address = message.value("address").toString();

        Status status;
        if (message.value("status") == "initializing")
            status = INITIALIZING;
        else if (message.value("status") == "client")
            status = CLIENT;
        else if (message.value("status") == "server")
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
            node = new MulticastProtocolNode(id, m_proxyConnection);
            m_nodes.append(node);
        }

        // update info
        if (node != m_currentNode)
            node->update(score, status, port, address, initialized);
    } else { // Different workspace
        ISession *session = m_proxyConnection->session(&parent);
        QVariantMap workspaces = session->value("workspaces").toMap();
        QVariantMap workspace;
        workspace.insert("name", workspaceName);
        workspace.insert("last_seen", QDateTime::currentDateTime());
        workspaces.insert(workspaceId, workspace);
        session->setValue("workspaces", workspaces);
    }
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
        availableClients.insert(m_nodes.at(i)->id(),
                                QString("%1:%2").arg(m_nodes.at(i)->address()).arg(m_nodes.at(i)->port()));
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
    QVariantMap message = m_currentNode->message();
    QObject parent;
    message.insert("num_available_clients", m_proxyConnection->session(&parent)->availableClients().count());

    return message;
}
