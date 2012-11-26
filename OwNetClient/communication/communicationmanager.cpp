#include "communicationmanager.h"
#include "communicationinstance.h"

#include <QDebug>

CommunicationManager *CommunicationManager::m_communicationManager = NULL;
const int CommunicationManager::expirationTimeInSeconds = 15;

CommunicationManager *CommunicationManager::getInstance()
{
    if (m_communicationManager == NULL)
        m_communicationManager = new CommunicationManager();

    return m_communicationManager;
}

CommunicationManager::CommunicationManager()
    : m_initializing(true)
{
    m_myId = "unique-client-id";
    m_myScore = 1;

    // add self as first instance
    CommunicationInstance *communicationInstance;
    communicationInstance = new CommunicationInstance(m_myId);
    m_instances.append(communicationInstance);
    communicationInstance->update(m_myScore, NONE);
}

QString CommunicationManager::myId() const
{
    return m_myId;
}

int CommunicationManager::myScore() const
{
    return m_myScore;
}

CommunicationManager::Status CommunicationManager::myStatus()
{
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

void CommunicationManager::initialized()
{
    m_initializing = false;
}

void CommunicationManager::processMessage(QVariantMap *message)
{
    CommunicationInstance *communicationInstance = NULL;

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
        communicationInstance = new CommunicationInstance(id);
        m_instances.append(communicationInstance);
    }

    // update info
    communicationInstance->update(score, status);
}

QList<CommunicationInstance *> &CommunicationManager::getCommunicationInstances()
{
    cleanAndSortInstances();

    return m_instances;
}

CommunicationInstance *CommunicationManager::getServer()
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

void CommunicationManager::cleanAndSortInstances()
{
    // clean expired proxies
    QMutableListIterator<CommunicationInstance *> i(m_instances);
    while (i.hasNext()) {
        if (i.next()->isExpired())
            i.remove();
    }

    // sort proxies by score
    qSort(m_instances.begin(), m_instances.end(), CommunicationInstance::lessThan);
}
