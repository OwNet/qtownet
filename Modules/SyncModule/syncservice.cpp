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
    if (request->action() == "get_changes_to_upload")
        return getChangesToUpload(bus, request);
    if (request->action() == "upload_changes")
        return uploadChanges(bus, request);
    return NULL;
}

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
    return new QVariant(server.getUpdates(clientRecordNumbers, syncAllGroups, clientId));
}

QVariant *SyncService::getChangesToUpload(IBus *, IRequest *request)
{
    bool ok = false;
    QVariantList clientRecordNumbers = request->postBodyFromJson(&ok).toList();
    if (!ok)
        return NULL;

    SyncServer server(m_proxyConnection);
    return new QVariant(server.getChangesToUpload(clientRecordNumbers));
}

QVariant *SyncService::uploadChanges(IBus *, IRequest *request)
{
    bool ok = false;
    QVariantList changes = request->postBodyFromJson(&ok).toList();
    if (!ok)
        return NULL;

    SyncServer server(m_proxyConnection);
    server.saveAndApplyReceivedUpdates(changes);
    QVariantMap success;
    success.insert("success", true);
    return new QVariant(success);
}
