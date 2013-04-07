#ifndef PREFETCHJOB_H
#define PREFETCHJOB_H


#include <QMutex>
#include <QMap>
#include <QWebView>
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

private:
    static const int CLEAN_THRESHOLD = 50;
    IProxyConnection *m_proxyConnection;
    bool prefetch();
    void resetWorker();
    void startWorker(QString &link);
    void tryClean();
    bool m_running;

    QThread *m_workerThread;
    QMutex m_activeMutex;

    int runs;
private slots:
    void workerCompleted();
};

#endif // PREFETCHJOB_H
