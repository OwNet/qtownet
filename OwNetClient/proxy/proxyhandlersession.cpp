#include "proxyhandlersession.h"

ProxyHandlerSession::ProxyHandlerSession(QObject *parent)
    : QObject(parent), m_lastDependentObjectId(0)
{
}

int ProxyHandlerSession::registerDependentObject()
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    int objectId = m_lastDependentObjectId++;
    m_dependentObjects.append(objectId);
    return objectId;
}

void ProxyHandlerSession::deregisterDependentObject(int objectId)
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    m_dependentObjects.removeAll(objectId);
    if (!m_dependentObjects.count())
        emit allFinished();
}

bool ProxyHandlerSession::hasDependentObjects()
{
    QMutexLocker mutexLocker(&m_dependentObjectsMutex);

    return m_dependentObjects.count() > 0;
}

void ProxyHandlerSession::forceQuitAll()
{
    emit shouldForceQuit();
}
