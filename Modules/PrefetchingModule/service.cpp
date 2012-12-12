#include "service.h"

#include "irequest.h"
#include "prefetchingmodule.h"
#include "prefetchjob.h"
#include "ibus.h"
#include "isession.h"
#include "irequest.h"
#include "idatabaseupdate.h"
#include "ibus.h"
#include "iproxyconnection.h"
#include "idatabaseselectquery.h"
#include "idatabaseselectquerywheregroup.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

Service::Service(IProxyConnection *proxyConnection, PrefetchingModule* parent) :
    QObject((QObject*)parent),
    m_proxyConnection(proxyConnection)
{
    m_pageCounter = 0;
    m_module = parent;
}


QVariant *Service::processRequest(IBus *bus, IRequest *req)
{
 //   MessageHelper::debug(QString("PREFETCH %1").arg(req->relativeUrl()));

    //QVariantMap response;
    QByteArray *ret = NULL;
    if (req->action() == "link") {
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
        return new QVariant(*ret);
    }

    return new QVariant("{ ERROR : 'PROBLEM'}");
}

QByteArray *Service::link(IBus *, IRequest *req)
{
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
        return new QByteArray(QString("<html><body onload=\"document.links[0].click();\"><a id=\"clickme\" href=\"%1\">prefetch this</a></body></html>").arg(req->parameterValue("page")).toLatin1());
    }
    return NULL;
}

QByteArray *Service::done(IBus *, IRequest *req) {
    if (req->hasParameter("page") && !req->parameterValue("page").isEmpty()) {
        // TODO

    }

    return NULL;
}
