#include "pingserver.h"

#include "multicastprotocol.h"
#include "multicastprotocolnode.h"

PingServer::PingServer(MulticastProtocol *multicastProtocol, QObject *parent) :
    QObject(parent),
    m_multicastProtocol(multicastProtocol)
{
}

void PingServer::updateAvailableClients(const QVariantList &clients)
{
    foreach (QVariant client, clients)
        m_multicastProtocol->processMessage(client.toMap());
}

void PingServer::updateClient(const QVariantMap &client)
{
    m_multicastProtocol->processMessage(client);
}

QVariantList PingServer::availableClients() const
{
    QVariantList clients;

    foreach (MulticastProtocolNode *node, m_multicastProtocol->nodes())
        clients.append(node->message());

    return clients;
}
