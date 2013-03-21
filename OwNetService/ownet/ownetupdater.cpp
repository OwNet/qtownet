#include "ownetupdater.h"
#include "ownetclient.h"
#include "ownetcloudserver.h"
#include "ownetinstallation.h"

#include "version.h"

#define QUAZIP_STATIC
#include <quazip.h>
#include <quazipfile.h>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

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

    // create Extracted/ dir
    QString extractedPath = "OwNet/Extracted";
    QDir extracted = QDir::temp();
    extracted.mkpath(extractedPath);
    extracted.cd(extractedPath);

    // extract files
    QuaZip zip(updateFile.fileName());
    zip.open(QuaZip::mdUnzip);

    QuaZipFile file(&zip);
    for(bool more=zip.goToFirstFile(); more; more=zip.goToNextFile())
    {
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();

        // create file directory
        extracted.mkpath(file.getActualFileName().replace(QRegularExpression("/[^/]+$"), ""));

        // write to file
        QFile outputFile(extracted.absoluteFilePath(file.getActualFileName()));
        outputFile.open(QFile::WriteOnly | QFile::Truncate);
        outputFile.write(data);
        outputFile.close();

        file.close();

    }
    zip.close();

    qDebug() << "Extracted files to " << extracted.absolutePath();

    // run update executable
    // TODO
}
