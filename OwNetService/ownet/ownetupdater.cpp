#include "ownetupdater.h"
#include "ownetclient.h"
#include "ownetcloudserver.h"
#include "ownetinstallation.h"

#include "version.h"

#include <QDebug>
#include <QDir>
#include <QFile>

OwNetUpdater::OwNetUpdater(OwNetClient *client, OwNetCloudServer *cloudServer, QObject *parent) :
    m_client(client), m_cloudServer(cloudServer), QObject(parent)
{
    connect(m_cloudServer, SIGNAL(updateMetadataReceived(QVariantMap)),
            this, SLOT(updateMetadataReceived(QVariantMap)));
    connect(m_cloudServer, SIGNAL(packageDataReceived(QString, QByteArray)),
            this, SLOT(packageDataReceived(QString, QByteArray)));
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

        qDebug() << "Downloading " << updateMetadata.value("last_file").toString();
        m_cloudServer->downloadUpdatePackage(updateMetadata.value("last_file").toString());
    }
}

void OwNetUpdater::packageDataReceived(QString fileName, QByteArray data)
{
    // create temp dir
    QString tempPath = "OwNet/Updates";
    QDir temp = QDir::temp();
    temp.mkpath(tempPath);
    temp.cd(tempPath);

    // write to file
    QFile updateFile(temp.absoluteFilePath(fileName));
    updateFile.open(QFile::WriteOnly | QFile::Truncate);
    updateFile.write(data);
    updateFile.close();

    qDebug() << "Package data written to " << updateFile.fileName();
}
