#include "syncservice.h"

#include "irequest.h"
#include "syncserver.h"

SyncService::SyncService(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

QVariant *SyncService::processRequest(IBus *bus, IRequest *request)
{
    if (request->action() == "get_updates")
        return getUpdates(bus, request);
    if (request->action() == "available_records")
        return availableRecords(bus, request);
    if (request->action() == "upload_changes")
        return uploadChanges(bus, request);
    return NULL;
}

/**
 * @brief Return new updates on the server to client
 * @param request
 * @return
 */
QVariant *SyncService::getUpdates(IBus *, IRequest *request)
{
    bool ok = false;
    QVariantMap requestBody = request->postBodyFromJson(&ok).toMap();
    if (!ok)
        return NULL;

    int clientId = requestBody.value("client_id", -1).toInt();
    if (clientId == -1)
        return NULL;

    bool syncAllGroups = requestBody.value("sync_all_groups", false).toBool();
    QVariantMap clientRecordNumbers = requestBody.value("client_record_numbers").toMap();

    SyncServer server(m_proxyConnection);
    return new QVariant(server.updates(clientRecordNumbers, syncAllGroups, clientId));
}

/**
 * @brief Get list of changes that are present on the server
 * @return
 */
QVariant *SyncService::availableRecords(IBus *, IRequest *)
{
    SyncServer server(m_proxyConnection);
    return new QVariant(server.clientRecordNumbers());
}

/**
 * @brief Upload new changes from client to server.
 * @param request
 * @return
 */
QVariant *SyncService::uploadChanges(IBus *, IRequest *request)
{
    bool ok = false;
    QVariantList changes = request->postBodyFromJson(&ok).toList();
    if (!ok)
        return NULL;

    SyncServer server(m_proxyConnection);
    server.saveAndApplyUpdates(changes);
    QVariantMap success;
    success.insert("success", true);
    return new QVariant(success);
}
