#include "pingjob.h"

#include "iproxyconnection.h"
#include "isession.h"
#include "pingserver.h"
#include "multicastprotocol.h"
#include "multicastprotocolnode.h"

#include <QSettings>

PingJob::PingJob(PingServer *server, IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection),
    m_pingServer(server)
{
}

void PingJob::execute()
{
    if (!m_pingMutex.tryLock())
        return;

    QObject parent;
    QString serviceName = "";
    if (!m_proxyConnection->session(&parent)->isServer())
        serviceName = "server";
    else if (!m_proxyConnection->settings(&parent)->value("custom_server_ip").toString().isEmpty())
        serviceName = "custom_server";
    else {
        m_pingMutex.unlock();
        return;
    }

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, serviceName, "ping/available_clients", this);
    QVariantMap message = m_pingServer->multicastProtocol()->currentNode()->message();
    message.insert("called_ip", m_proxyConnection->settings(&parent)->value("custom_server_ip").toString());
    request->setPostBody(message);
    IResponse *response = m_proxyConnection->callModule(request);
    if (response->status() == IResponse::OK)
        m_pingServer->updateAvailableClients(response->body().toList());

    m_pingMutex.unlock();
}
