#ifndef PROXYHANDLERSESSION_H
#define PROXYHANDLERSESSION_H

#include <QObject>
#include <QMutex>

class IProxyMockable;

/**
 * @brief Represents the current session in ProxyHandler, which is open until all dependent objects finish.
 * Tells the ProxyHandler when to finish (emits a signal).
 */
class ProxyHandlerSession : public QObject
{
    Q_OBJECT

public:
    explicit ProxyHandlerSession(QObject *parent = 0);

    int registerDependentObject();
    void deregisterDependentObject(int objectId);
    bool hasDependentObjects();

    void forceQuitAll();

signals:
    void allFinished();
    void shouldForceQuit();

private:
    QList<int> m_dependentObjects;
    int m_lastDependentObjectId;
    QMutex m_dependentObjectsMutex;
};

#endif // PROXYHANDLERSESSION_H
