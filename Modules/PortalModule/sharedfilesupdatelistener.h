#ifndef SHAREDFILESUPDATELISTENER_H
#define SHAREDFILESUPDATELISTENER_H

#include "idatabaseupdatelistener.h"
#include "ijobaction.h"

#include <QStringList>

class QNetworkAccessManager;
class QNetworkReply;

class SharedFilesUpdateListener : public IJobAction, public IDatabaseUpdateListener
{
    Q_OBJECT
public:
    SharedFilesUpdateListener();

    /// Job actions
    int interval() { return 1000; }
    void execute();
    void setProxyConnection(IProxyConnection *proxyConnection);

    /// Listener actions
    QString tableName() const { return "shared_files"; }
    void tableUpdated(IDatabaseUpdateQuery *query);

signals:
    void newSharedFileSig(const QString &url);

private slots:
    void newSharedFile(const QString &url);
    void downloadFinished(QNetworkReply *reply);

private:
    IProxyConnection *m_proxyConnection;
    QNetworkAccessManager *m_networkAccessManager;

    QStringList m_urlsToDownload;
};

#endif // SHAREDFILESUPDATELISTENER_H
