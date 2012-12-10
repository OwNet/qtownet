#include "syncservice.h"

#include "irequest.h"
#include "syncserver.h"
#include "irouter.h"

SyncService::SyncService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void SyncService::init(IRouter *router)
{
    router->addRoute("/get_updates")
            ->on(IRequest::POST, ROUTE(getUpdates));
}

/**
 * @brief Return new updates on the server to client
 * @param request
 * @return
 */
IResponse *SyncService::getUpdates(IRequest *request)
{
    bool ok = false;
    QVariantMap requestBody = request->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    int clientId = requestBody.value("client_id", -1).toUInt();
    if (clientId == -1)
        return NULL;

    bool syncAllGroups = requestBody.value("sync_all_groups", false).toBool();
    QVariantMap clientRecordNumbers = requestBody.value("client_record_numbers").toMap();

    SyncServer server(m_proxyConnection);

    return request->response(server.updates(clientRecordNumbers, syncAllGroups, clientId));
}
