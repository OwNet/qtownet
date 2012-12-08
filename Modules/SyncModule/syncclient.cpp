#include "syncclient.h"

#include "synclock.h"
#include "iproxyconnection.h"
#include "idatabasesettings.h"
#include "idatabaseselectquery.h"

SyncClient::SyncClient(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void SyncClient::update()
{
    QObject parent;
    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "server", "sync/get_updates", -1, this);

    QVariantMap body;
    body.insert("client_id", m_proxyConnection->databaseSettings(this)->value("client_id"));
    body.insert("sync_all_groups", true);

    QVariantMap clientRecordNumbers;
    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_sync_records", &parent);
    while (query->next()) {
        QVariantMap groupClients;
        if (clientRecordNumbers.contains(query->value("group_id").toString()))
            groupClients = clientRecordNumbers.value(query->value("group_id").toString()).toMap();

        groupClients.insert(query->value("client_id").toString(), query->value("last_client_rec_num").toInt());
        clientRecordNumbers.insert(query->value("group_id").toString(), groupClients);
    }
    body.insert("client_record_numbers", clientRecordNumbers);

    request->setPostBody(body);
    m_proxyConnection->callModule(request);
}
