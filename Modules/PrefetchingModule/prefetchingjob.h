#ifndef PREFETCHJOB_H
#define PREFETCHJOB_H
#include <QMutex>

#include "ijobaction.h"

class IProxyConnection;
class BrowserWorker;

class PrefetchingJob : public IJobAction
{
    Q_OBJECT
public:
    PrefetchingJob();

    int interval() { return 3 * 60 * 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection) { m_proxyConnection = proxyConnection; }
    bool createSeparateThread() const { return false; }

private:
    static const int CLEAN_THRESHOLD = 2;
    IProxyConnection *m_proxyConnection;
    bool prefetch();
    void startWorker(QString &link);
    void tryClean();
    bool m_running;

    QMutex m_activeMutex;

    int runs;
private slots:
    void workerCompleted();
};

#endif // PREFETCHJOB_H
