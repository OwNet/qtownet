#include "ownetupdater.h"
#include "ownetclient.h"
#include "ownetcloudserver.h"
#include "ownetinstallation.h"

#include "version.h"

#include <QDebug>

OwNetUpdater::OwNetUpdater(OwNetClient *client, OwNetCloudServer *cloudServer, QObject *parent) :
    m_client(client), m_cloudServer(cloudServer), QObject(parent)
{
    connect(m_cloudServer, SIGNAL(updateMetadataReceived(QVariantMap)),
            this, SLOT(updateMetadataReceived(QVariantMap)));
}

void OwNetUpdater::checkForUpdates()
{
    m_cloudServer->checkUpdateMetadata();
}

void OwNetUpdater::updateMetadataReceived(QVariantMap updateMetadata)
{
    Version lastVersion(updateMetadata.value("last_version").toString().toStdString());

    OwNetInstallation installation(this);
    Version currentVersion(installation.currentVersion().toStdString());

    if (currentVersion < lastVersion)
    {
        qDebug() << "Update available " << installation.currentVersion()
                 << " -> " << updateMetadata.value("last_version").toString();
    }
}
