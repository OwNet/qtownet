#include "prefetchingmodule.h"
#include <QStringList>

#include "prefetchjob.h"

PrefetchingModule::PrefetchingModule(QObject *parent) : IModule(parent)
{
    setUrl("prefetch");
    m_pageCounter = 0;
    m_prefetchJob = new PrefetchJob();
}


QByteArray* PrefetchingModule::processRequest(IBus *bus, ProxyRequest *req)
{
    MessageHelper::debug(QString("PREFETCH %1").arg(req->relativeUrl()));

    if (req->action() == "visit")
    {
        if (req->hasParameter("page"))
        {

            QString page = req->parameterValue("page");

            int id = m_pageCounter++;

            m_prefetchJob->registerPage(id,  page);


            MessageHelper::debug(QString("prefetch:registered %0").arg(page));

            if (req->hasParameter("ref"))
            {
                page = req->parameterValue("ref");
                if (!page.isEmpty())
                {
                    m_prefetchJob->removePage(page);
                    MessageHelper::debug(QString("prefetch:removed %0").arg(page));
                }
            }

            return new QByteArray(QString("owNetPAGEID = %0;").arg(QString::number(id)).toAscii());
        }
    }
    else if (req->action() == "link")
    {
        if (req->hasParameter("from") && req->hasParameter("to"))
        {

            QString page = req->parameterValue("from");
            QString linkUrl = req->parameterValue("to");
            bool ok = false;
            int pageId = page.toInt(&ok, 10);
            if (ok)
            {
               // m_map.value(pageId)->addLinks(linkUrl);
                m_prefetchJob->registerLink(pageId, linkUrl);
                MessageHelper::debug(QString("PREFETCH:Added for %0 : %1").arg(page, linkUrl));
            }
       }
    }
    else if (req->action() == "close")
    {
        if (req->hasParameter("page"))
        {
            QString page = req->parameterValue("page");
            bool ok = false;
            int pageId = page.toInt(&ok, 10);
            if (ok)
            {
                m_prefetchJob->removePage(pageId);
                MessageHelper::debug(QString("PREFETCH: Removed %0").arg(page));
            }
        }
    }

    return new QByteArray("{ OK : 'OK'}");
}
