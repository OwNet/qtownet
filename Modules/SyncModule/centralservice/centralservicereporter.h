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

    bool reportSyncJournal();
    bool reportBrowsingHistory();

private:
    QVariantMap getServerSyncState(bool *ok = NULL);
    bool sendUpdates(const QVariantMap &message);
    bool sendHistory(const QVariantList &history);
    QString workspaceId() const;

    IProxyConnection *m_proxyConnection;
};

#endif // CENTRALSERVICEREPORTER_H
