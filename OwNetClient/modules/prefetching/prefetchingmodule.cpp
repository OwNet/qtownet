#include "prefetchingmodule.h"
#include <QStringList>

PrefetchingModule::PrefetchingModule(QObject *parent) : IModule(parent)
{
    setUrl("prefetch");
    m_reg = new QRegExp(".*prefetch.ownet/(.*)");
}


QByteArray* PrefetchingModule::processRequest(IBus *bus, ProxyRequest *req)
{
    MessageHelper::debug(QString("PREFETCH %1").arg(req->relativeUrl()));

    if (req->relativeUrl().startsWith("visit"))
    {
        if (req->parameterValue("page") != NULL )
        {

            QString page = req->parameterValue("page");

            QString idString = req->parameterValue("id");

            m_map.insert(idString.toInt(), new LoggedPage(idString.toInt(), page));

            MessageHelper::debug(QString("prefetch:registered %0").arg(page));
        }
    }
    else if (req->relativeUrl().startsWith("link"))
    {
        if (req->parameterValue("from") != NULL  && req->parameterValue("to") != NULL)
        {

            QString page = req->parameterValue("from");
            QString linkUrl = req->parameterValue("to");
            bool ok = false;
            int pageId = page.toInt(&ok, 10);
            if (ok && m_map.contains(pageId))
            {
                m_map.value(pageId)->addLinks(linkUrl);
                MessageHelper::debug(QString("PREFETCH:Added for %0 : %1").arg(page, linkUrl));
            }
       }
    }

    return new QByteArray("{ OK : 'OK'}");
}
