#include "prefetchingmodule.h"
#include <QStringList>

#include "prefetchjob.h"

PrefetchingModule::PrefetchingModule(QObject *parent) : IModule(parent)
{
    setUrl("prefetch");
    m_reg = new QRegExp(".*prefetch.ownet/(.*)");
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

            QString idString = req->parameterValue("id");

            m_prefetchJob->registerPage(idString.toInt(),  page);

            MessageHelper::debug(QString("prefetch:registered %0").arg(page));
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

    return new QByteArray("{ OK : 'OK'}");
}
