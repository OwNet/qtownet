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
    MulticastProtocolNode *communicationInstance;
    communicationInstance = new MulticastProtocolNode(m_myId);
    m_instances.append(communicationInstance);
    communicationInstance->update(m_myScore, NONE);
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
    cleanAndSortInstances();

    if (m_initializing)
        return INITIALIZING; // not initialized yet

    // find first after INITIALIZING
    for (int i = 0; i < m_instances.size(); ++i)
    {
        if (m_instances.at(i)->status() != INITIALIZING)
        {
            if (m_instances.at(i)->id() == myId())
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
    for (int i = 0; i < m_instances.size(); ++i)
    {
        if (m_instances.at(i)->id() == id)
        {
            communicationInstance = m_instances.at(i);
            break;
        }
    }

    // or create new
    if (! communicationInstance)
    {
        communicationInstance = new MulticastProtocolNode(id);
        m_instances.append(communicationInstance);
    }

    // update info
    communicationInstance->update(score, status);
}

QList<MulticastProtocolNode *> &MulticastProtocol::getCommunicationInstances()
{
    cleanAndSortInstances();

    return m_instances;
}

MulticastProtocolNode *MulticastProtocol::getServer()
{
    cleanAndSortInstances();

    if (m_instances.empty())
        return NULL; // no other proxies
    else
    {
        if (m_instances.first()->status() == INITIALIZING)
            return NULL; // future server initializing
        else
            return m_instances.first(); // best is the server
    }
}

void MulticastProtocol::cleanAndSortInstances()
{
    // clean expired proxies
    QMutableListIterator<MulticastProtocolNode *> i(m_instances);
    while (i.hasNext()) {
        if (i.next()->isExpired())
            i.remove();
    }

    // sort proxies by score
    qSort(m_instances.begin(), m_instances.end(), MulticastProtocolNode::lessThan);
}
