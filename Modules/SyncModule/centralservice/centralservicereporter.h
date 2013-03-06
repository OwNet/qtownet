#ifndef CENTRALSERVICEREPORTER_H
#define CENTRALSERVICEREPORTER_H

#include <QObject>

class IProxyConnection;

class CentralServiceReporter : public QObject
{
    enum {
        MaxUpdatesInReport = 100
    };
    Q_OBJECT
public:
    explicit CentralServiceReporter(IProxyConnection *proxyConnection, QObject *parent = 0);

    bool report();

private:
    QVariantMap getServerSyncState(bool *ok = NULL);
    bool sendUpdates(const QVariantMap &message);
    QString workspaceId() const;

    IProxyConnection *m_proxyConnection;
};

#endif // CENTRALSERVICEREPORTER_H
