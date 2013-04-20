#include "centralservicereporter.h"

#include "irequest.h"
#include "iproxyconnection.h"
#include "syncserver.h"
#include "isession.h"
#include "idatabaseselectquery.h"
#include "idatabasesettings.h"
#include "idatabaseselectqueryjoin.h"
#include "idatabaseselectquerywheregroup.h"

#include <QDateTime>
#include <QSettings>
#include <QDebug>

CentralServiceReporter::CentralServiceReporter(IProxyConnection *proxyConnection, QObject *parent) :
    QObject(parent),
    m_proxyConnection(proxyConnection)
{
}

/**
 * @brief If is currently a server, reports updates in sync journal to the central services.
 * @return True if success
 */
bool CentralServiceReporter::reportSyncJournal()
{
    QObject parent;
    if (!m_proxyConnection->session(&parent)->isServer())
        return true;

    bool ok;
    QVariantMap serverState = getServerSyncState(&ok);
    if (!ok) {
        m_proxyConnection->message("Central Service Report: Failed to access the service");
        return false;
    }

    SyncServer syncServer(m_proxyConnection);
    QVariantList updates = syncServer.updates(serverState, true, QString());
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
        m_proxyConnection->message("Central Service Report: Failed to report updates to the service");
    return success;
}

/**
 * @brief Reports the visited websites to the central services.
 * @return True if success
 */
bool CentralServiceReporter::reportBrowsingHistory()
{
    QVariantList history;
    IDatabaseSettings *settings = m_proxyConnection->databaseSettings(this);

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("pages", this);
    QString lastReport = settings->value("last_central_service_report", "");
    IDatabaseSelectQueryWhereGroup *joinOn = query->join("user_visits_pages")->whereGroup(IDatabaseSelectQuery::And);
    joinOn->where("user_visits_pages.page_id", "pages.id", IDatabaseSelectQuery::Equal, false);
    if (lastReport != "")
        joinOn->where("user_visits_pages.date_created", lastReport, IDatabaseSelectQuery::GreaterThan);
    query->select("pages.id, pages.absolute_uri, user_visits_pages.date_created");
    query->limit(500);

    QString currentDate = QDateTime::currentDateTime().toString(Qt::ISODate);

    while (query->next()) {
        QVariantMap item;
        item.insert("cache_id", query->value("id").toUInt());
        item.insert("absolute_uri", query->value("absolute_uri").toString());
        item.insert("accessed_at", query->value("date_created").toString());
        history.append(item);
    }
    if (!history.count())
        return true;

    if (sendHistory(history)) {
        settings->setValue("last_central_service_report", currentDate);
        return true;
    }
    m_proxyConnection->message("Central Service Report: Failed to report browsing hitory to the service");
    return false;
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

bool CentralServiceReporter::sendHistory(const QVariantList &history)
{
    QVariantMap report;
    report.insert("workspace_id", workspaceId());
    report.insert("history", history);

    IRequest *request = m_proxyConnection->createRequest(IRequest::POST,
                                                         "central_service",
                                                         "Sync/ReportHistory",
                                                         this);
    request->setPostBody(report);

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
