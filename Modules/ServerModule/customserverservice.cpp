#include "customserverservice.h"

#include "irequest.h"
#include "clientservicecall.h"
#include "irouter.h"
#include "iproxyconnection.h"
#include "isettings.h"

#include <QStringList>

CustomServerService::CustomServerService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void CustomServerService::init(IRouter *router)
{
    router->setDefaultRoute(DEFAULT_ROUTE_FN {
        QObject parent;
        ISettings *settings = m_proxyConnection->settings(&parent);
        if (settings->value("custom_server_ip").isNull() || settings->value("custom_server_port").isNull())
            return req->response(IResponse::SERVICE_UNAVAILABLE);

        QString ip = settings->value("custom_server_ip").toString();
        int port = settings->value("custom_server_port").toInt();

        QStringList urlSplit = req->relativeUrl().split("/");
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();
        if (urlSplit.isEmpty()) return req->response();
        urlSplit.takeFirst();

        return ClientServiceCall(m_proxyConnection)
                                .callClientService(ip, port, urlSplit.join("/"), req);
    });
}
