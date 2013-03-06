#include "centralservicereporter.h"

#include "irequest.h"
#include "iproxyconnection.h"
#include "syncserver.h"
#include "isession.h"

#include <QSettings>
#include <QDebug>

CentralServiceReporter::CentralServiceReporter(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

bool CentralServiceReporter::report()
{
    QObject parent;
    if (!m_proxyConnection->session(&parent)->isServer())
        return true;

    bool ok;
    QVariantMap serverState = getServerSyncState(&ok);
    if (!ok) {
        m_proxyConnection->debugMessage("Central Service Report: Failed to access the service");
        return false;
    }
    foreach (QString key, serverState.keys()) {
        foreach (QString p, serverState.value(key).toMap().keys()) {
            int val = serverState.value(key).toMap().value(p).toInt();
            qDebug() << val;
        }
    }

    SyncServer syncServer(m_proxyConnection);
    QVariantList updates = syncServer.updates(serverState, true, -1);
    if (!updates.count())
        return true;

    QSettings *settings = m_proxyConnection->settings(&parent);
    settings->beginGroup("current_workspace");

    QVariantMap message;
    message.insert("workspace_id", settings->value("id").toString());
    message.insert("workspace_name", settings->value("name").toString());

    bool success = true;
    if (updates.count() < MaxUpdatesInReport) {
        message.insert("updates", updates);
        success = sendUpdates(message);
    } else {
        for (int i = 0; i < updates.count() && success; i += MaxUpdatesInReport) {
            message.insert("updates", updates.mid(i, MaxUpdatesInReport));
            success = sendUpdates(message);
        }
    }
    if (!success)
        m_proxyConnection->debugMessage("Central Service Report: Failed to report updates to the service");
    return success;
}

QVariantMap CentralServiceReporter::getServerSyncState(bool *ok)
{
    if (ok) *ok = false;

    IRequest *request = m_proxyConnection->createRequest(IRequest::GET,
                                                         "central_service",
                                                         QString("Sync/%1/GetSyncState")
                                                         .arg(workspaceId()),
                                                         this);

    IResponse *response = m_proxyConnection->callModule(request);
    if (response->status() != IResponse::OK || !response->body().isValid())
        return QVariantMap();

    if (ok) *ok = true;
    return response->body().toMap();
}

bool CentralServiceReporter::sendUpdates(const QVariantMap &message)
{
    IRequest *request = m_proxyConnection->createRequest(IRequest::POST,
                                                         "central_service",
                                                         "Sync/ReportUpdates",
                                                         this);
    request->setPostBody(message);

    IResponse *response = m_proxyConnection->callModule(request);
    return response->status() == IResponse::OK;
}

QString CentralServiceReporter::workspaceId() const
{
    QObject parent;
    QSettings *settings = m_proxyConnection->settings(&parent);
    settings->beginGroup("current_workspace");
    return settings->value("id").toString();
}
