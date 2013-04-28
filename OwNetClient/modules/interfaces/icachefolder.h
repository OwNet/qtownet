#ifndef ICACHEFOLDER_H
#define ICACHEFOLDER_H

#include <QString>

class QFile;
class QObject;

class ICacheFolder
{
public:
    virtual QFile *cacheFile(const QString &url, int part, QObject *parent = NULL) const = 0;
    virtual QFile *cacheFile(uint id, int part, QObject *parent = NULL) const = 0;
};

#endif // ICACHEFOLDER_H
