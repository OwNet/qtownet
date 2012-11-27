#include "service.h"

#include "irequest.h"
#include "prefetchingmodule.h"
#include "prefetchjob.h"

Service::Service(IProxyConnection *proxyConnection, PrefetchingModule* parent) :
    QObject((QObject*)parent),
    m_proxyConnection(proxyConnection)
{
    m_pageCounter = 0;
    m_module = parent;
}

//QVariant *Service::index(IBus *, IRequest *request)
//{
//    QVariantMap values;
//    values.insert("module", request->module());
//    return new QVariant(values);
//}

QByteArray *Service::processRequest(IBus *, IRequest *req)
{
 //   MessageHelper::debug(QString("PREFETCH %1").arg(req->relativeUrl()));

    //QVariantMap response;

    if (req->action() == "visit")
    {
        if (req->hasParameter("page"))
        {
            QString page = req->parameterValue("page");

            int id = m_pageCounter++;

            m_module->prefetchJob()->registerPage(id,  page);


            //MessageHelper::debug(QString("prefetch:registered %0").arg(page));

            if (req->hasParameter("ref"))
            {
                page = req->parameterValue("ref");
                if (!page.isEmpty())
                {
                    m_module->prefetchJob()->removePage(page);
                   // MessageHelper::debug(QString("prefetch:removed %0").arg(page));
                }
            }


            return new QByteArray(QString("owNetPAGEID = %1;").arg(QString::number(id)).toAscii());
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
                m_module->prefetchJob()->registerLink(pageId, linkUrl);
               // MessageHelper::debug(QString("PREFETCH:Added for %0 : %1").arg(page, linkUrl));
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
                m_module->prefetchJob()->removePage(pageId);
                //MessageHelper::debug(QString("PREFETCH: Removed %0").arg(page));
            }
        }
    }

    return new QByteArray("{ OK : 'OK'}");
}

