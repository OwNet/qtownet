#include "cachefolder.h"

#include "applicationdatastorage.h"
#include "proxyrequest.h"

CacheFolder::CacheFolder()
    : QDir()
{
    QDir appDataDir = ApplicationDataStorage().appDataDirectory();
    if (!appDataDir.exists("cache")) {
        appDataDir.mkdir("cache");
    }
    setPath(appDataDir.absoluteFilePath("cache"));
}

QFile *CacheFolder::cacheFile(ProxyRequest *request, int part, QObject *parent)
{
    return new QFile(absoluteFilePath(QString("%1-%2.cache")
                            .arg(request->hashCode())
                            .arg(part)), parent);
}
