#ifndef CACHEFOLDER_H
#define CACHEFOLDER_H

#include <QDir>

#include "icachefolder.h"

class ProxyRequest;

class CacheFolder : public QDir, public ICacheFolder
{
public:
    CacheFolder();

    long size();
    long maximumSize() { return 60 * 1000 * 1000; } // 500 MB
    long cacheReserveSize();

    QFile *cacheFile(const QString &url, int part, QObject *parent = NULL) const;
    QFile *cacheFile(ProxyRequest *request, int part, QObject *parent = NULL) const;
    QFile *cacheFile(uint id, int part, QObject *parent = NULL) const;
};

#endif // CACHEFOLDER_H
