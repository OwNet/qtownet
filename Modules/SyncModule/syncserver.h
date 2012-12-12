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

    QVariantList updates(const QVariantMap &clientRecordNumbers, bool syncAllGroups, int requestingClientId);
    QVariantMap clientRecordNumbers();
    void saveAndApplyUpdates(const QVariantList &changes);

private:
    IProxyConnection *m_proxyConnection;
};

#endif // SYNCSERVER_H
