#include "prefetchjob.h"


//#include <QSqlQuery>
#include <QDateTime>
//#include <QSqlRecord>
//#include <QVariant>


#include "Visit.h"
#include "WebPage.h"
#include "WebPageManager.h"
#include <QStringList>
#include "SetProxy.h"


PrefetchJob::PrefetchJob(QObject *parent)
    : Job(5 * 1000, parent)
{
//    wpm = new WebPageManager();
//    QNetworkProxy proxy(QNetworkProxy::HttpProxy,
//                        QString("127.0.0.1"),
//                        8081,
//                        QString(""),
//                        QString(""));
//    wpm->currentPage()->networkAccessManager()->setProxy(proxy);
}

void PrefetchJob::execute()
{
    if (!m_activeMutex.tryLock())
        return;

    prefetch();

    m_activeMutex.unlock();
}

void PrefetchJob::prefetch()
{
//    if (wpm->currentPage()->history()->canGoBack())
//        wpm->currentPage()->history()->back();
//    wpm->currentPage()->currentFrame()->load(QUrl(QString("http://www.sme.sk/")));
}
