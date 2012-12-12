#include "serverservice.h"

#include "irequest.h"
#include "clientservicecall.h"
#include "irouter.h"

#include <QStringList>

ServerService::ServerService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void ServerService::init(IRouter *router)
{
    router->setDefaultRoute(DEFAULT_ROUTE_FN {
        QStringList urlSplit = req->relativeUrl().split("/");
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();

        return req->response(ClientServiceCall(m_proxyConnection).callClientService(0, urlSplit.join("/"), req));
    });
}
