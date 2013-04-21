#include "cachefolder.h"

#include "applicationdatastorage.h"
#include "proxyrequest.h"
#include "applicationenvironment.h"

#include <QDebug>

CacheFolder::CacheFolder()
    : QDir()
{
    QDir appDataDir;
    if (ApplicationEnvironment().contains("CACHE_FOLDER")) {
        appDataDir.setPath(ApplicationEnvironment().value("CACHE_FOLDER"));
        qDebug() << appDataDir.absolutePath();
    } else {
        appDataDir = ApplicationDataStorage().appDataDirectory();
        if (!appDataDir.exists("cache"))
            appDataDir.mkdir("cache");
    }
    setPath(appDataDir.absoluteFilePath("cache"));
}

long CacheFolder::size()
{
    long size = 0;

    QFileInfoList fileInfos = entryInfoList(QDir::Files);
    for (int i = 0; i < fileInfos.count(); ++i)
        size += fileInfos.at(i).size();

    return size;
}

long CacheFolder::cacheReserveSize()
{
    return maximumSize() / 10;
}

QFile *CacheFolder::cacheFile(ProxyRequest *request, int part, QObject *parent)
{
    return cacheFile(request->hashCode(), part, parent);
}

QFile *CacheFolder::cacheFile(uint id, int part, QObject *parent)
{
    return new QFile(absoluteFilePath(QString("%1-%2.cache")
                            .arg(id)
                            .arg(part)), parent);
}
