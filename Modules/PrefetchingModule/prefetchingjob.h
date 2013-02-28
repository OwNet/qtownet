#ifndef PREFETCHJOB_H
#define PREFETCHJOB_H


#include <QMutex>
#include <QMap>
#include <QWebView>
#include <QObject>
#include "ijobaction.h"
class IProxyConnection;
class BrowserWorker;



class PrefetchingJob : public QObject, public IJobAction
{
    Q_OBJECT
public:
    PrefetchingJob(IProxyConnection *proxyConnection, QObject* parent);

    int interval() { return 10 * 1000; }
    void execute();

private:
    IProxyConnection *m_proxyConnection;
    bool prefetch();
    void resetWorker();
    void startWorker(QString &link);

    bool m_running;

    QThread *m_workerThread;
    QMutex m_activeMutex;
private slots:
    void workerCompleted();
};

#endif // PREFETCHJOB_H
