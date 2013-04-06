#include "pingserver.h"

#include "multicastprotocol.h"
#include "multicastprotocolnode.h"
#include "iproxyconnection.h"
#include "idatabasesettings.h"

PingServer::PingServer(MulticastProtocol *multicastProtocol, IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_multicastProtocol(multicastProtocol),
    m_proxyConnection(proxyConnection)
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

QVariantList PingServer::availableClients(const QString &myIp) const
{
    QVariantList clients;
    QObject parent;
    QString myId = m_proxyConnection->databaseSettings(&parent)->clientId();

    foreach (MulticastProtocolNode *node, m_multicastProtocol->nodes()) {
        QVariantMap message = node->message();
        if (message.value("id").toString() == myId)
            message["address"] = myIp;
        else
            message["address"] = node->address();
        clients.append(message);
    }

    return clients;
}
