#ifndef SYNCSERVER_H
#define SYNCSERVER_H

#include <QObject>
#include <QVariant>
#include <QMutex>

class IProxyConnection;

class SyncServer : public QObject
{
    Q_OBJECT
public:
    static SyncServer *instance() {
        static QMutex mutex;
        if (!m_instance) {
            mutex.lock();
            if (!m_instance)
                m_instance = new SyncServer;
            mutex.unlock();
        }

        return m_instance;
    }

    void setProxyConnection(IProxyConnection *proxyConnection) { m_proxyConnection = proxyConnection; }

    QVariantList getUpdates(const QVariantMap &clientRecordNumbers, bool syncAllGroups, int clientId);
    QVariantList getChangesToUpload(const QVariantList &clientRecordNumbers);
    void uploadChanges(const QVariantList &changes);

private:
    SyncServer();

    IProxyConnection *m_proxyConnection;
    QMutex m_syncLock;

    static SyncServer *m_instance;
};

#endif // SYNCSERVER_H
