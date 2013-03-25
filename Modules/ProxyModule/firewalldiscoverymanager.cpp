#include "firewalldiscoverymanager.h"

#include "irequest.h"
#include "iproxyconnection.h"
#include "idatabasesettings.h"
#include "isession.h"

#include <QSettings>
#include <QThread>
#include <QTimer>

bool FirewallDiscoveryManager::m_wasPingedBack = false;
bool FirewallDiscoveryManager::m_statusChecked = false;

FirewallDiscoveryManager::FirewallDiscoveryManager(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

void FirewallDiscoveryManager::ping(const QString &clientId)
{
    m_clientToPingId = clientId;

    QThread* thread = new QThread;
    this->moveToThread(thread);
    connect(thread, SIGNAL(started()), this, SLOT(startPing()));
    connect(this, SIGNAL(finishedPing()), thread, SLOT(quit()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

void FirewallDiscoveryManager::checkFirewallStatus()
{
    if (m_statusChecked)
        return;

    QObject parent;
    ISession *session = m_proxyConnection->session(&parent);
    QString myId = m_proxyConnection->databaseSettings(&parent)->clientId();
    QVariantMap availableClients = session->availableClients();

    m_statusChecked = true;

    foreach (QString id, availableClients.keys()) {
        if (id == myId)
            continue;

        IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "clients", QString("%1/firewall/ping_me?my_id=%2")
                                                             .arg(id)
                                                             .arg(myId), &parent);
        if (m_proxyConnection->callModule(request)->status() == IResponse::OK) {
            QTimer::singleShot(5 * 1000, this, SLOT(checkPingResponse()));
            return;
        }
    }

    m_statusChecked = false;
}

void FirewallDiscoveryManager::setPingedBack(bool pingedBack)
{
    m_wasPingedBack = pingedBack;
}

void FirewallDiscoveryManager::startPing()
{
    IRequest *request = m_proxyConnection->createRequest(IRequest::POST, "clients", QString("%1/firewall/ping")
                                                         .arg(m_clientToPingId), this);
    m_proxyConnection->callModule(request);
    emit finishedPing();
}

void FirewallDiscoveryManager::checkPingResponse()
{
    if (!m_wasPingedBack) {
        QObject parent;
        int port = m_proxyConnection->settings(&parent)->value("application/listen_port", 8081).toInt();
        m_proxyConnection->message(tr("Firewall was detected that prevents OwNet from fully functioning. Please deactivate the firewall on port %1").arg(port), tr("Firewall Detected"), IProxyConnection::CriticalPopup);
    }
}
