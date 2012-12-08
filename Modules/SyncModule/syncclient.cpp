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
void SyncClient::update()
{
    QObject parent;
    SyncServer server(m_proxyConnection);

    QVariantMap body;
    body.insert("client_id", m_proxyConnection->databaseSettings(this)->value("client_id"));
    body.insert("sync_all_groups", true);
    body.insert("client_record_numbers", server.clientRecordNumbers());

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "server", "sync/get_updates", this);
    request->setPostBody(body);
    IResponse *response = m_proxyConnection->callModule(request);
    if (response->status() != IResponse::OK)
        return;

    server.saveAndApplyUpdates(response->body().toList());
}

/**
 * @brief Send new updates to server.
 */
void SyncClient::reportToServer()
{
    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "server", "sync/available_records", this);
    IResponse *response = m_proxyConnection->callModule(request);
    if (response->status() != IResponse::OK)
        return;

    QVariantMap currentItemsOnServer = response->body().toMap();
    SyncServer server(m_proxyConnection);
    QVariantList uploadItems = server.updates(currentItemsOnServer, true, 0);

    if (uploadItems.count()) {
        request = m_proxyConnection->createRequest(IRequest::POST, "server", "sync/upload_changes", this);
        request->setPostBody(uploadItems);
        m_proxyConnection->callModule(request);
    }
}
