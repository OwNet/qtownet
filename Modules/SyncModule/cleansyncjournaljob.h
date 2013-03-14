#ifndef CLEANSYNCJOURNALJOB_H
#define CLEANSYNCJOURNALJOB_H

#include <QObject>
#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;

class CleanSyncJournalJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    enum {
        MaxNumberOfCacheItems = 5000
    };
    explicit CleanSyncJournalJob(IProxyConnection *proxyConnection, QObject *parent = 0);

    int interval() { return 30 * 1000; }

    void execute();

private:
    QMutex m_cleanMutex;
    IProxyConnection *m_proxyConnection;
};

#endif // CLEANSYNCJOURNALJOB_H
