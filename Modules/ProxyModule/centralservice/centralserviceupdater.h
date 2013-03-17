#ifndef CENTRALSERVICEUPDATER_H
#define CENTRALSERVICEUPDATER_H

#include <QObject>

class IProxyConnection;

class CentralServiceUpdater : public QObject
{
    Q_OBJECT
public:
    explicit CentralServiceUpdater(IProxyConnection *proxyConnection, QObject *parent = 0);

    bool updateJournal();

private:
    QString workspaceId() const;

    IProxyConnection *m_proxyConnection;
};

#endif // CENTRALSERVICEUPDATER_H
