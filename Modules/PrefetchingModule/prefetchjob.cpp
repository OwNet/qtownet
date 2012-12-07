#include "prefetchjob.h"

#include <QNetworkProxy>
//#include <QSqlQuery>
#include <QDateTime>
//#include <QSqlRecord>
//#include <QVariant>
//#include "messagehelper.h"

#include <QStringList>




#include "loggedpage.h"
PrefetchJob::PrefetchJob()
{
    m_last = -1;
    QNetworkProxy proxy(QNetworkProxy::HttpProxy,
                        QString("127.0.0.1"),
                        8081,
                        QString(""),
                        QString(""));

    connect(&m_webView, SIGNAL(loadFinished(bool)), this, SLOT(resetBrowser(bool)));
    m_webView.page()->networkAccessManager()->setProxy(proxy);
}

void PrefetchJob::resetBrowser(bool)
{
    m_webView.load(QUrl("about:blank"));
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
       // MessageHelper::debug(QString("PREFETCH:Added for %0 : %1").arg(QString("" + id), url));
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

void PrefetchJob::removePage(QString url)
{
    int id = -1;
    for (QMap<int, LoggedPage*>::iterator it = m_map.begin(); it != m_map.end();)
    {
        if (((LoggedPage*) it.value())->url() == url)
        {
            id = it.key();
            break;
        }
        ++it;
    }

    if (id != -1)
    {
        removePage(id);
    }
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
        link = QString(QUrl(m_map.value(m_last)->link()).toEncoded());
    }
    m_mapMutex.unlock();


    if (!link.isEmpty())
    {
        link = QString("http://prefetch.ownet/api/prefetch/load?page=%1").arg(link);

        m_webView.load(QUrl(link));
    }
}