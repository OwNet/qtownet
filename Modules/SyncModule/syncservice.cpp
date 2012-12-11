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
    router->addRoute("/available_records")
            ->on(IRequest::GET, ROUTE(availableRecords));
    router->addRoute("/upload_changes")
            ->on(IRequest::POST, ROUTE(uploadChanges));
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

/**
 * @brief Get list of changes that are present on the server
 * @return
 */
IResponse *SyncService::availableRecords(IRequest *request)
{
    SyncServer server(m_proxyConnection);

    return request->response(server.clientRecordNumbers());
}

/**
 * @brief Upload new changes from client to server.
 * @param request
 * @return
 */
IResponse *SyncService::uploadChanges(IRequest *request)
{
    bool ok = false;
    QVariantList changes = request->postBodyFromJson(&ok).toList();
    if (!ok)
        return NULL;

    SyncServer server(m_proxyConnection);
    server.saveAndApplyUpdates(changes);

    return request->response(IResponse::OK);
}
