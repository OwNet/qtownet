#ifndef SYNCSERVER_H
#define SYNCSERVER_H

#include <QObject>
#include <QVariant>

class IProxyConnection;

class SyncServer : public QObject
{
    Q_OBJECT
public:
    SyncServer(IProxyConnection *proxyConnection, QObject *parent = 0);

    QVariantList getUpdates(const QVariantMap &clientRecordNumbers, bool syncAllGroups, int clientId);
    QVariantList getChangesToUpload(const QVariantList &clientRecordNumbers);
    void saveAndApplyReceivedUpdates(const QVariantList &changes);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SYNCSERVER_H
