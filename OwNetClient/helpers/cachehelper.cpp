#include "cachehelper.h"

#include <QHash>

CacheHelper::CacheHelper(QObject *parent) :
    QObject(parent)
{
}

uint CacheHelper::cacheId(const QString &url)
{
    return qHash(url.toLower());
}
