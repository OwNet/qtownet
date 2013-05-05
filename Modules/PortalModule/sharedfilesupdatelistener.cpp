#include "sharedfilesupdatelistener.h"

#include "idatabaseupdatequery.h"
#include "iproxyconnection.h"
#include "isession.h"
#include "idatabaseselectquery.h"
#include "idatabasesettings.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QSettings>

SharedFilesUpdateListener::SharedFilesUpdateListener() :
    m_networkAccessManager(NULL),
    m_proxyConnection(NULL)
{
}

void SharedFilesUpdateListener::execute()
{
    QStringList urls(m_urlsToDownload);
    m_urlsToDownload.clear();
    foreach (QString url, urls) {
        m_networkAccessManager->get(QNetworkRequest(QUrl(url)));
    }
}

void SharedFilesUpdateListener::setProxyConnection(IProxyConnection *proxyConnection)
{
    m_proxyConnection = proxyConnection;

    int port = m_proxyConnection->settings()->value("application/multicast_port", 8081).toInt();

    m_networkAccessManager = new QNetworkAccessManager(this);
    m_networkAccessManager->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, "localhost", port));
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    connect(this, SIGNAL(newSharedFileSig(QString)), this, SLOT(newSharedFile(QString)), Qt::QueuedConnection);
}

void SharedFilesUpdateListener::tableUpdated(IDatabaseUpdateQuery *query)
{
    if (query->type() == IDatabaseUpdateQuery::InsertOrUpdate) {
        QVariantList columns = query->columnsForListeners();
        for (int i = 0; i < columns.count(); ++i) {
            emit newSharedFileSig(columns.at(i).toMap().value("url").toString());
        }
    }
}

void SharedFilesUpdateListener::newSharedFile(const QString &url)
{
    QObject parent;
    if (!m_proxyConnection->session(&parent)->isServer())
        return;

    IDatabaseSelectQuery *query = m_proxyConnection->databaseSelect("client_caches", &parent);
    query->singleWhere("cache_id", m_proxyConnection->cacheId(url));
    QString myClientId = m_proxyConnection->databaseSettings(&parent)->clientId();

    QVariantMap availableClients = m_proxyConnection->session(&parent)->availableClients();
    bool foundAvailableClient = false;
    while (query->next()) {
        if (query->value("client_id").toString() == myClientId)
            return;

        if (availableClients.contains(query->value("client_id").toString())) {
            foundAvailableClient = true;
            break;
        }
    }

    if (foundAvailableClient)
        m_urlsToDownload.append(url);
}

void SharedFilesUpdateListener::downloadFinished(QNetworkReply *reply)
{
    reply->deleteLater();
}
