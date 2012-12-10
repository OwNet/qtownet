#include "syncclient.h"

#include "iproxyconnection.h"
#include "idatabasesettings.h"
#include "idatabaseselectquery.h"
#include "syncserver.h"

SyncClient::SyncClient(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

/**
 * @brief Update journal from server.
 */
void SyncClient::updateFromServer()
{
    downloadUpdatesFromClient(1);
}

/**
 * @brief Used by the server to download updates from all online clients
 */
void SyncClient::updateFromClients()
{
    downloadUpdatesFromClient(1);
}

/**
 * @brief Download sync updates from the specified client
 * @param clientId ID of the client
 */
void SyncClient::downloadUpdatesFromClient(int clientId)
{
    QObject parent;
    SyncServer server(m_proxyConnection);

    QVariantMap body;
    body.insert("client_id", m_proxyConnection->databaseSettings(this)->value("client_id"));
    body.insert("sync_all_groups", true);
    body.insert("client_record_numbers", server.clientRecordNumbers());

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "clients", QString("%1/sync/get_updates")
                                                         .arg(clientId), this);
    request->setPostBody(body);
    IResponse *response = m_proxyConnection->callModule(request);
    if (response->status() != IResponse::OK)
        return;

    server.saveAndApplyUpdates(response->body().toList());
}
