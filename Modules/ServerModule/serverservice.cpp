#include "serverservice.h"

#include "irequest.h"
#include "clientservicecall.h"
#include "irouter.h"
#include "iproxyconnection.h"
#include "isession.h"

#include <QStringList>

ServerService::ServerService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void ServerService::init(IRouter *router)
{
    router->setDefaultRoute(DEFAULT_ROUTE_FN {
        QObject parent;
        ISession *session = m_proxyConnection->session(&parent);
        if (session->value("server_id").isNull())
            return req->response(IResponse::SERVICE_UNAVAILABLE);

        QStringList urlSplit = req->relativeUrl().split("/");
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();

        return ClientServiceCall(m_proxyConnection)
                                .callClientService(
                                    session->serverId(),
                                    urlSplit.join("/"), req);
    });
}
