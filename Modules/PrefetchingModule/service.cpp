#include "service.h"

#include "irequest.h"
#include "prefetchingmodule.h"
#include "prefetchjob.h"
#include "ibus.h"

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

QByteArray *Service::processRequest(IBus *bus, IRequest *req)
{
 //   MessageHelper::debug(QString("PREFETCH %1").arg(req->relativeUrl()));

    //QVariantMap response;
    QByteArray *ret = NULL;
    if (req->action() == "visit") {
        ret = visit(bus,req);
    } else if (req->action() == "link") {
        ret = link(bus,req);
    } else if (req->action() == "close") {
        ret = close(bus, req);
    }
    else if (req->action() == "load") {
        ret = load(bus,req);
    }
    else if (req->action() == "done") {
        ret = done(bus,req);
    }

    if (ret) {
        return ret;
    }

    return new QByteArray("{ OK : 'OK'}");
}


QByteArray *Service::visit(IBus *, IRequest *req)
{
    if (req->hasParameter("page")) {
        QString page = req->parameterValue("page");

        int id = m_pageCounter++;

        m_module->prefetchJob()->registerPage(id,  page);


        //MessageHelper::debug(QString("prefetch:registered %0").arg(page));

        if (req->hasParameter("ref")) {
            page = req->parameterValue("ref");
            if (!page.isEmpty())
            {
                m_module->prefetchJob()->removePage(page);
               // MessageHelper::debug(QString("prefetch:removed %0").arg(page));
            }
        }

        return new QByteArray(QString("owNetPAGEID = %1;").arg(QString::number(id)).toAscii());
    }
    return NULL;
}

QByteArray *Service::link(IBus *, IRequest *req) {
    if (req->hasParameter("from") && req->hasParameter("to")) {

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
   return NULL;
}

QByteArray *Service::close(IBus *, IRequest *req) {
    if (req->hasParameter("page")) {
        QString page = req->parameterValue("page");
        bool ok = false;
        int pageId = page.toInt(&ok, 10);
        if (ok)
        {
            m_module->prefetchJob()->removePage(pageId);
            //MessageHelper::debug(QString("PREFETCH: Removed %0").arg(page));
        }
    }
    return NULL;
}

QByteArray *Service::load(IBus *bus, IRequest *req) {
    if (req->hasParameter("page") && !req->parameterValue("page").isEmpty()) {
        bus->setContentType("text/html");
        return new QByteArray(QString("<html><body onload=\"document.links[0].click();\"><a id=\"clickme\" href=\"%1\">prefetch this</a></body></html>").arg(req->parameterValue("page")).toAscii());
    }
    return NULL;
}

QByteArray *Service::done(IBus *, IRequest *req) {
    if (req->hasParameter("page") && !req->parameterValue("page").isEmpty()) {
        // TODO

    }

    return NULL;
}
