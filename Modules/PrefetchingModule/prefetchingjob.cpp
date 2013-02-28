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
}


void PrefetchingJob::execute()
{
    if (!m_activeMutex.tryLock()) {
        return;
    }

    if (m_running == true) {
        m_activeMutex.unlock();
        return;
    }
    m_running = true;

    m_activeMutex.unlock();

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

    link = "http://www.mtbiker.sk";
    if (!link.isEmpty())
    {

        startWorker(link);
        return true;
    }
    return false;
}

void PrefetchingJob::startWorker(QString &link) {
    m_workerThread = new QThread(this);
    //m_workerThread = new BrowserWorker(link, m_workerThread);
    BrowserWorker *worker = new BrowserWorker(link, m_workerThread);
   // m_workerThread->doWork();
    connect(m_workerThread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(m_workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
//        connect(m_workerThread, SIGNAL(loadCompleted()), this, SLOT(workerCompleted()));
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


