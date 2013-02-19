#include "syncservice.h"

#include "irequest.h"
#include "syncserver.h"
#include "syncclient.h"
#include "irouter.h"
#include "iproxyconnection.h"
#include "isession.h"

SyncService::SyncService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void SyncService::init(IRouter *router)
{
    router->addRoute("/get_updates")
            ->on(IRequest::POST, ROUTE(getUpdates));
    router->addRoute("/now")
            ->on(IRequest::GET, ROUTE(syncNow));
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
        return request->response(IResponse::BAD_REQUEST);

    int clientId = requestBody.value("client_id", -1).toUInt();
    if (clientId == -1)
        return request->response(IResponse::BAD_REQUEST);

    bool syncAllGroups = requestBody.value("sync_all_groups", false).toBool();
    QVariantMap clientRecordNumbers = requestBody.value("client_record_numbers").toMap();

    SyncServer server(m_proxyConnection);

    return request->response(server.updates(clientRecordNumbers, syncAllGroups, clientId));
}

/**
 * @brief Sync to server and sync with other clients
 * @param request
 * @return
 */
IResponse *SyncService::syncNow(IRequest *request)
{
    SyncClient client(m_proxyConnection);
    QVariantMap json;

    json.insert("number_of_available_clients", m_proxyConnection->session(&client)->availableClients().count());
    json.insert("updated_from_server", client.updateFromServer());
    json.insert("updates_from_clients", client.updateFromClients());

    return request->response(json);
}
