#include "firewalldiscoverymanager.h"

#include "irequest.h"
#include "iproxyconnection.h"
#include "idatabasesettings.h"
#include "isession.h"
#include "isettings.h"

#include <QThread>
#include <QTimer>

bool FirewallDiscoveryManager::m_wasPingedBack = false;
bool FirewallDiscoveryManager::m_statusChecked = false;

FirewallDiscoveryManager::FirewallDiscoveryManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
    connect(this, SIGNAL(finishedPing()), this, SLOT(deleteLater()));
}

void FirewallDiscoveryManager::ping(const QString &clientId)
{
    m_clientToPingId = clientId;

    QThread* thread = new QThread;
    this->moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(startPing()));
    connect(this, SIGNAL(finishedPing()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void FirewallDiscoveryManager::checkFirewallStatus()
{
    if (m_statusChecked) {
        emit finishedPing();
        return;
    }

    QObject parent;
    QString myId = m_proxyConnection->databaseSettings(&parent)->clientId();
    QVariantMap availableClients = m_proxyConnection->session(&parent)->availableClients();

    m_statusChecked = true;

    foreach (QString id, availableClients.keys()) {
        if (id == myId)
            continue;

        IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "clients", QString("%1/firewall/ping_me?my_id=%2")
                                                             .arg(id)
                                                             .arg(myId), &parent);
        if (m_proxyConnection->callModule(request)->status() == IResponse::OK) {
            QTimer::singleShot(5 * 1000, this, SLOT(checkPingResponse()));
            return;
        }
    }

    m_statusChecked = false;
    emit finishedPing();
}

void FirewallDiscoveryManager::setPingedBack(bool pingedBack)
{
    m_wasPingedBack = pingedBack;
}

void FirewallDiscoveryManager::startPing()
{
    IRequest *request = m_proxyConnection->createRequest(IRequest::GET, "clients", QString("%1/firewall/ping")
                                                         .arg(m_clientToPingId), this);
    m_proxyConnection->callModule(request);
    emit finishedPing();
}

void FirewallDiscoveryManager::checkPingResponse()
{
    if (!m_wasPingedBack) {
        QObject parent;
        int port = m_proxyConnection->settings(&parent)->listenPort();
        m_proxyConnection->message(tr("Firewall was detected that prevents OwNet from functioning properly. Please deactivate the firewall on port %1.").arg(port), tr("Firewall Detected"), IProxyConnection::CriticalPopup);
    }
    emit finishedPing();
}
