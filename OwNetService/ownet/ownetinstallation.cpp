#include "ownetinstallation.h"

#include "jsondocument/jsondocument.h"
#include "settings.h"
#include <QDir>
#include <QDebug>

OwNetInstallation::OwNetInstallation(QObject *parent) :
    QObject(parent)
{
    // read <resources>/version.json
    QDir dir;
    dir.setPath(Settings().value("application/resources_folder_path").toString());
    QFile file(dir.absoluteFilePath("version.json"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray json = file.readAll();

    // parse json
    bool ok = false;
    QVariantMap currentMetadata = JsonDocument::fromJson(json, &ok).toVariant().toMap();
    if (ok)
    {
        m_currentVersion = currentMetadata.value("current_version").toString();
    }
    else
        qDebug() << "Problem reading <resources_folder_path>/version.json";
}

QString OwNetInstallation::currentVersion()
{
    return m_currentVersion;
}
