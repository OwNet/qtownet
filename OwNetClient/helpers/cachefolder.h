#ifndef CACHEFOLDER_H
#define CACHEFOLDER_H

#include <QDir>

class ProxyRequest;

class CacheFolder : public QDir
{
public:
    CacheFolder();

    QFile *cacheFile(ProxyRequest *request, int part, QObject *parent = NULL);
};

#endif // CACHEFOLDER_H
