#include "cachefolder.h"

#include "applicationdatastorage.h"
#include "proxyrequest.h"
#include "applicationenvironment.h"
#include "cachehelper.h"

#include <QDebug>

CacheFolder::CacheFolder()
    : QDir()
{
    if (ApplicationEnvironment().contains("CACHE_FOLDER")) {
        setPath(ApplicationEnvironment().value("CACHE_FOLDER"));
    } else {
        QDir appDataDir;
        appDataDir = ApplicationDataStorage().appDataDirectory();
        if (!appDataDir.exists("cache"))
            appDataDir.mkdir("cache");
        setPath(appDataDir.absoluteFilePath("cache"));
    }
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

QFile *CacheFolder::cacheFile(const QString &url, int part, QObject *parent) const
{
    return cacheFile(CacheHelper::cacheId(url), part, parent);
}

QFile *CacheFolder::cacheFile(ProxyRequest *request, int part, QObject *parent) const
{
    return cacheFile(request->hashCode(), part, parent);
}

QFile *CacheFolder::cacheFile(uint id, int part, QObject *parent) const
{
    return new QFile(absoluteFilePath(QString("%1-%2.cache")
                            .arg(id)
                            .arg(part)));
}
