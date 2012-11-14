#include "prefetchjob.h"


//#include <QSqlQuery>
#include <QDateTime>
//#include <QSqlRecord>
//#include <QVariant>
#include "messagehelper.h"

#include <QStringList>


#include "loggedpage.h"
PrefetchJob::PrefetchJob(QObject *parent)
    : Job(2 * 60 * 1000, parent)
{
    m_last = -1;
    QNetworkProxy proxy(QNetworkProxy::HttpProxy,
                        QString("127.0.0.1"),
                        8081,
                        QString(""),
                        QString(""));


    m_webView.page()->networkAccessManager()->setProxy(proxy);
}

void PrefetchJob::registerPage(int id, QString url)
{
    m_mapMutex.lock();
    if (!m_map.contains(id))
    {
        m_last = id;
        m_map.insert(id, new LoggedPage(id, url));
    }
    m_mapMutex.unlock();
}

void PrefetchJob::registerLink(int id, QString url)
{
    m_mapMutex.lock();
    if (m_map.contains(id))
    {
        m_map.value(id)->addLinks(url);
        MessageHelper::debug(QString("PREFETCH:Added for %0 : %1").arg(QString("" + id), url));
    }
    m_mapMutex.unlock();
}

void PrefetchJob::removePage(int id)
{
    m_mapMutex.lock();
    if (m_map.contains(id))
    {
        delete m_map.value(id);
        m_map.remove(id);
    }
    m_mapMutex.unlock();
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
    m_mapMutex.lock();

    QString link;
    if (!m_map.empty() && m_map.contains(m_last))
    {
        link = m_map.value(m_last)->link();
    }
    m_mapMutex.unlock();


    if (!link.isEmpty())
    {
        m_webView.load(QUrl(link));
    }
}
