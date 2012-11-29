#include "proxyhandlersession.h"

ProxyHandlerSession::ProxyHandlerSession(QObject *parent)
    : QObject(parent), m_lastDependentObjectId(0)
{
}

/**
 * @brief Register a dependent object to wait for.
 * @return ID of the objecy in session
 */
int ProxyHandlerSession::registerDependentObject()
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    int objectId = m_lastDependentObjectId++;
    m_dependentObjects.append(objectId);
    return objectId;
}

/**
 * @brief Deregisters the dependent object.
 * If there are no dependent objects left, triggers signal to close ProxyHandler.
 * @param objectId ID of the object in session
 */
void ProxyHandlerSession::deregisterDependentObject(int objectId)
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    m_dependentObjects.removeAll(objectId);
    if (!m_dependentObjects.count())
        emit allFinished();
}

/**
 * @brief Returns true if there are any dependent objects left
 * @return True if has dependent objects
 */
bool ProxyHandlerSession::hasDependentObjects()
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    return m_dependentObjects.count() > 0;
}

/**
 * @brief Emits signal to close dependent objects.
 * Not all dependent objects might listen to the signal.
 */
void ProxyHandlerSession::forceQuitAll()
{
    emit shouldForceQuit();
}
