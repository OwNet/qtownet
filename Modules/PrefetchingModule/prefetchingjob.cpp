#include "prefetchingjob.h"

#include <QNetworkProxy>
//#include <QSqlQuery>
#include <QDateTime>
//#include <QSqlRecord>
//#include <QVariant>
//#include "messagehelper.h"

#include <QStringList>

#include "iproxyconnection.h"
#include "idatabaseselectquery.h"
#include "idatabaseupdatequery.h"
#include "idatabaseselectquerywheregroup.h"
#include <QDebug>
#include <QThread>

#include "browserworker.h"
PrefetchingJob::PrefetchingJob(IProxyConnection *proxyConnection, QObject* parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    m_workerThread = NULL;
    m_running = false;
    runs = 0;
}


void PrefetchingJob::execute()
{
    if (!m_activeMutex.tryLock()) {
        return;
    }

    if (m_running == true || m_proxyConnection->lastConnectionTraffic() > 50) {
        m_activeMutex.unlock();
        return;
    }

    m_running = true;
    runs += 1;
    m_activeMutex.unlock();

    tryClean();

    bool running = prefetch();

    if (!running) {
        m_activeMutex.lock();
        m_running = false;
        m_activeMutex.unlock();
    }
}

void PrefetchingJob::resetWorker() {
    if (m_workerThread != NULL) {
        try {
            if (m_workerThread->isRunning()) {
                m_workerThread->terminate();
            }
            delete m_workerThread;

        } catch (std::exception& e) {
            qDebug()<<e.what();
        }

        m_workerThread = NULL;
    }
}

void PrefetchingJob::tryClean() {
    if (runs > CLEAN_THRESHOLD) {
        QObject parent;

        IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
        select->select("priority");
        select->orderBy("priority desc");
        select->offset(10);
        select->limit(1);
        if (select->next()) {
            bool ok = false;
            int priority = select->value("priority").toInt(&ok);
            if (ok) {
                IDatabaseUpdateQuery *query = m_proxyConnection->databaseUpdateQuery("prefetch_orders", &parent, false);
                query->setType(IDatabaseUpdateQuery::Delete);
                query->singleWhere("priority", priority, IDatabaseSelectQuery::LessThan);
                query->executeQuery();
            }
        }

        runs = 1;
    }
}

bool PrefetchingJob::prefetch()
{
    QObject parent;
    // check running lock
    // check connection status and traffic
    resetWorker();

    qDebug("Attempting to start prefetching.");
    // IDatabaseUpdate *update = m_proxyConnection->databaseUpdate(&parent);
    IDatabaseSelectQuery *select = m_proxyConnection->databaseSelect("prefetch_orders", &parent);
    select->orderBy("priority desc");
    select->select("absolute_uri");

    QString link = "";
    if (select->next()) {
         link = select->value("absolute_uri").toString();
    }

    if (!link.isEmpty())
    {
        startWorker(link);
        return true;
    }
    return false;
}

void PrefetchingJob::startWorker(QString &link) {
    m_workerThread = new QThread(this);
    BrowserWorker *worker = new BrowserWorker(link, m_workerThread);
    connect(m_workerThread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(m_workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(worker, SIGNAL(workCompleted()), this, SLOT(workerCompleted()));

    m_workerThread->start();
}

void PrefetchingJob::workerCompleted() {
    m_activeMutex.lock();
    m_running = false;
    m_activeMutex.unlock();

    resetWorker();
}

//void MyObject::putWorkerInAThread()
//{
//    Worker *worker = new Worker;
//    QThread *workerThread = new QThread(this);

//    connect(workerThread, SIGNAL(started()), worker, SLOT(doWork()));
//    connect(workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
//    worker->moveToThread(workerThread);

//    // Starts an event loop, and emits workerThread->started()
//    workerThread->start();
//}


