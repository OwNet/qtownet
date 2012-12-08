#include "clientservice.h"

#include "irequest.h"
#include "clientservicecall.h"
#include "irouter.h"

#include <QStringList>

ClientService::ClientService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void ClientService::init(IRouter *router)
{
    router->setDefaultRoute(DEFAULT_ROUTE_FN {
        QStringList urlSplit = req->relativeUrl().split("/");
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();
        if (urlSplit.isEmpty()) return req->response();
        int id = urlSplit.takeFirst().toInt();

        return req->response(ClientServiceCall(m_proxyConnection).callClientService(id, urlSplit.join("/"), req));
    });
}
