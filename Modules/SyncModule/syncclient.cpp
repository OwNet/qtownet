#include "syncclient.h"

#include "iproxyconnection.h"
#include "idatabasesettings.h"
#include "idatabaseselectquery.h"
#include "syncserver.h"
#include "isession.h"
#include "synclock.h"

#include <QDebug>

SyncClient::SyncClient(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

/**
 * @brief Update journal from server.
 */
bool SyncClient::updateFromServer()
{
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    if (session->isServer())
        return false;

    return downloadUpdatesFromClient(session->serverId());
}

/**
 * @brief Used by the server to download updates from all online clients
 */
int SyncClient::updateFromClients()
{
    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    if (!session->isServer())
        return 0;

    int clients = 0;

    foreach (QString clientId, session->availableClients().keys())
        if (downloadUpdatesFromClient(clientId))
            clients++;

    return clients;
}

/**
 * @brief Download updates from clients if is server, or from server.
 */
void SyncClient::sync()
{
    updateFromClients();
    updateFromServer();
}

/**
 * @brief Download sync updates from the specified client
 * @param clientId ID of the client
 */
bool SyncClient::downloadUpdatesFromClient(const QString &clientId)
{
    QObject parent;
    if (clientId == m_proxyConnection->databaseSettings(&parent)->clientId())
        return false;

    qDebug() << "Sync with client " << clientId << " started";

    SyncServer server(m_proxyConnection);

    QVariantMap body;
    body.insert("client_id", m_proxyConnection->databaseSettings(this)->value("client_id"));
    body.insert("sync_all_groups", true);
    body.insert("client_record_numbers", server.clientRecordNumbers());

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "clients", QString("%1/sync/get_updates")
                                                         .arg(clientId), this);
    request->setPostBody(body);
    IResponse *response = m_proxyConnection->callModule(request);
    if (response->status() != IResponse::OK) {
        qDebug() << "Sync with client " << clientId << " failed";
        return false;
    }

    server.saveAndApplyUpdates(response->body().toList());
    qDebug() << "Sync with client " << clientId << " finished";

    return true;
}
