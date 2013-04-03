#include "centralserviceupdater.h"

#include "iproxyconnection.h"
#include "isession.h"
#include "syncserver.h"

#include <QSettings>

CentralServiceUpdater::CentralServiceUpdater(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

/**
 * @brief Update journal from the central service. Only done by the current server.
 * @return True if successful
 */
bool CentralServiceUpdater::updateJournal()
{
    QObject parent;
    if (!m_proxyConnection->session(&parent)->isServer())
        return true;

    SyncServer server(m_proxyConnection);
    QVariantMap clientRecordNumbers = server.clientRecordNumbers();
    if (clientRecordNumbers.count() == 0)
        return false;

    QVariantMap body;
    body.insert("workspace_id", workspaceId());
    body.insert("sync_all_groups", true);
    body.insert("client_record_numbers", clientRecordNumbers);
    IRequest *request = m_proxyConnection->createRequest(IRequest::POST,
                                                         "central_service",
                                                         "Sync/GetUpdates",
                                                         this);
    request->setPostBody(body);

    IResponse *response = m_proxyConnection->callModule(request);
    if (response->status() != IResponse::OK) {
        m_proxyConnection->message("Central Service Update: Failed to retrieve journal updates from the service");
        return false;
    }

    server.saveAndApplyUpdates(response->body().toList());
    return true;
}

QString CentralServiceUpdater::workspaceId() const
{
    QObject parent;
    QSettings *settings = m_proxyConnection->settings(&parent);
    settings->beginGroup("current_workspace");
    return settings->value("id").toString();
}
