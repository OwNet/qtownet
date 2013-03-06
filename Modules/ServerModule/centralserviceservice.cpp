#include "centralserviceservice.h"

#include "irouter.h"
#include "clientservicecall.h"

#include <QStringList>

CentralServiceService::CentralServiceService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void CentralServiceService::init(IRouter *router)
{
    router->setDefaultRoute(DEFAULT_ROUTE_FN {
        QStringList urlSplit = req->relativeUrl().split("/");
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();

        return ClientServiceCall(m_proxyConnection)
                                .callCentralService(urlSplit.join("/"), req);
    });
}
