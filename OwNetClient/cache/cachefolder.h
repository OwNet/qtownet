#ifndef CACHEFOLDER_H
#define CACHEFOLDER_H

#include <QDir>

class ProxyRequest;

class CacheFolder : public QDir
{
public:
    CacheFolder();

    long size();
    long maximumSize() { return 500 * 1000 * 1000; } // 500 MB
    long cacheReserveSize();

    QFile *cacheFile(ProxyRequest *request, int part, QObject *parent = NULL);
    QFile *cacheFile(int id, int part, QObject *parent = NULL);
};

#endif // CACHEFOLDER_H
